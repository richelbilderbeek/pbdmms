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

#include "kewe_individual.h"
#include "kewe_parameters.h"
#include "kewe_results.h"
#include "kewe_SES.h"
#include "kewe_simulation.h"

bool attractive_enough(
    const indiv& m,
    const indiv& f,
    const kewe_parameters& p,
    std::mt19937& gen
    )
{
  std::uniform_real_distribution<> dis(0, 1);

  return dis(gen) < calc_attractiveness(m.get_fem_pref(), f.get_male_trait(), p);
}

  bool fitness_high_enough(
      const indiv& i,
      const double comp_i,
      const indiv& j,
      const double comp_j,
      const kewe_parameters& parameters,
      std::mt19937& gen
      )
{
  std::uniform_real_distribution<> dis(0, 1);

  return dis(gen) < calc_survivability(i, comp_i, parameters)
      && dis(gen) < calc_survivability(j, comp_j, parameters);
}

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
      if(i!=j){comp+=gauss(pop[i].get_eco_trait()-pop[j].get_eco_trait(),p.sim_parameters.sc);}
    }
  return comp;
}

double calc_survivability(const indiv& m, const double comp, const kewe_parameters& p)
{
  return (1.0 - (comp / (p.sim_parameters.popsize * 2)) / gauss(m.get_eco_trait(), p.sim_parameters.sk))
        *(0.5+0.5*gauss(m.get_male_trait(),p.sim_parameters.sq));
}

double calc_attractiveness(
    const double pref,
    const double trait,
    const kewe_parameters& parameters
    )
{
  return gauss((pref - trait), parameters.sim_parameters.sm);
      //* gauss(mother.get_eco_trait() - father.get_eco_trait(), parameters.sim_parameters.se);
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

      /// If fitness parents is high enough, mate
      if (fitness_high_enough(pop[m], comp_m, pop[f], comp_f, parameters, gen))
        {
           indiv mother = pop[m];
           indiv father = pop[f];

      ///Check if they want to mate
           if (attractive_enough(mother, father, parameters, gen))
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

