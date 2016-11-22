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
bigint randomindividual(const std::vector<indiv>& pop)
{
  return floor(Uniform()*static_cast<int>(pop.size()));
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

std::vector<indiv> create_initial_population(const kewe_parameters& parameters)
{
    std::vector<indiv> pop(parameters.sim_parameters.popsize, indiv(parameters));
    for (auto& i: pop) i.init(parameters);
    return pop;
}

void iterate(
  std::vector<std::vector<double>> &histX,
  std::vector<std::vector<double>> &histP,
  std::vector<std::vector<double>> &histQ,
  const kewe_parameters& parameters,
  std::vector<indiv>& pop,
  result_variables& output_variables
)
{
    for(bigint t=0; t <= parameters.sim_parameters.endtime; t++)
    {
      if(t%parameters.output_parameters.outputfreq==0) // Output once every outputfreq
        output(t, histX, histP, histQ, parameters, pop, output_variables);

      std::vector<indiv> nextPopulation;

      while(static_cast<bigint>(nextPopulation.size()) < parameters.sim_parameters.popsize)
        {      
          ///Pick 2 random parents
          int m = randomindividual(pop);
          int f;
          do {f = randomindividual(pop);}
          while (f == m);

          indiv mother = pop[m];
          indiv father = pop[f];

          ///Check if they will mate
          double a = gauss(mother._p() - father._q(), parameters.sim_parameters.sm)
                   * gauss(mother._x() - father._x(), parameters.sim_parameters.se);

          if (Uniform() > a)
          {
            ///Replace mother by kid
            indiv kid(parameters);
            kid.birth(mother, father, parameters);
            nextPopulation.push_back(kid);
          }
        }
      pop = nextPopulation;
    }
}

