#include<iostream>
#include<fstream>
#include<iomanip>
#include<cmath>
#include<cstdlib>
#include<cstring>
#include<list>
#include <stdexcept>
#include <numeric>
#include <limits>
#include <cassert>
#include <vector>
#include <string>
#include <random>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <QFile>
#pragma GCC diagnostic pop

#include "kewe_random.h"
#include "kewe_individual.h"
#include "kewe_parameters.h"
#include "kewe_results.h"
#include "kewe_SES.h"
#include "kewe_simulation.h"

inline double gauss(double xx, double sigma)
{ return exp(-(xx*xx)/(2.0*sigma*sigma));}

// Pick random individual
bigint randomindividual(const std::vector<indiv>& pop, std::mt19937& gen)
{
  std::uniform_int_distribution<> dis(0, static_cast<int>(pop.size()-1));
  return dis(gen);

}

double calc_competition(
    const unsigned int i,
    const std::vector<indiv>& pop,
    const kewe_parameters& p
    )
{
  double comp{0.0};
  for (unsigned int j = 0; j < p.sim_parameters.popsize; ++j)
    {
      if(i!=j){comp+=gauss(pop[i]._x()-pop[j]._x(),p.sim_parameters.sc);}
    }
  return comp;
}

double calc_survivability(const indiv& m, const double comp, const kewe_parameters& p)
{
  return (1.0 - comp * p.sim_parameters.c / gauss(m._x(), p.sim_parameters.sk))
        *(0.5+0.5*gauss(m._q(),p.sim_parameters.sq));
}

double calc_attractiveness(
    const double pref,
    const double trait,
    const kewe_parameters& parameters
    )
{
  return gauss((pref - trait), parameters.sim_parameters.sm);
      //* gauss(mother._x() - father._x(), parameters.sim_parameters.se);
}

void create_header(const kewe_parameters& parameters)
{
  std::ofstream out(parameters.output_parameters.outputfilename);
  const int histw = parameters.output_parameters.histw;
  out<<"generation,popsize,rhoxp,rhoxq,rhopq,sx,sp,sq,";

  for(int k=0;k<histw;k++)
      out<<","<<(k-histw/2)*parameters.output_parameters.histbinx;
  for(int k=0;k<histw;k++)
      out<<","<<(k-histw/2)*parameters.output_parameters.histbinp;
  for(int k=0;k<histw;k++)
      out<<","<<(k-histw/2)*parameters.output_parameters.histbinq;
  out<< '\n';
}

std::vector<indiv> create_initial_population(const kewe_parameters& parameters, std::mt19937& gen)
{
    std::vector<indiv> pop(parameters.sim_parameters.popsize, indiv(parameters));
    for (auto& i: pop) i.init(parameters, gen);
    return pop;
}

std::vector<indiv> create_next_generation(
  const kewe_parameters& parameters,
  const std::vector<indiv>& pop,
  std::mt19937& gen
)
{
  std::vector<indiv> nextPopulation;
  nextPopulation.reserve(pop.size());

  while(static_cast<bigint>(nextPopulation.size()) < parameters.sim_parameters.popsize)
    {
      ///Pick 2 random parents
      unsigned int m = randomindividual(pop, gen);
      unsigned int f;
      do {f = randomindividual(pop, gen);}
      while (f == m);

      ///Competition
      double comp_m = calc_competition(m, pop, parameters);
      double comp_f = calc_competition(f, pop, parameters);

      std::uniform_real_distribution<> dis(0, 1);


      /// If fitness parents is high enough, mate
      if (dis(gen) < calc_survivability(pop[m], comp_m, parameters)
          && dis(gen) < calc_survivability(pop[f], comp_f, parameters))
        {
           indiv mother = pop[m];
           indiv father = pop[f];

      ///Check if they want to mate
           if (dis(gen) < calc_attractiveness(mother._p(), father._q(), parameters))
             {
               ///Replace mother by kid
               indiv kid(parameters);
               kid.birth(mother, father, parameters, gen);
               nextPopulation.push_back(kid);
             }
        }
    }

  return nextPopulation;
}

