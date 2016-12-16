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

  return dis(gen) < calc_attractiveness(m, f, p);
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

  return dis(gen) < calc_survivability_indiv(i, comp_i, parameters)
      && dis(gen) < calc_survivability_indiv(j, comp_j, parameters);
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
      assert(i < static_cast<unsigned int>(pop.size()));
      assert(j < static_cast<unsigned int>(pop.size()));
      if(i!=j){comp+=gauss(pop[i].get_eco_trait()-pop[j].get_eco_trait(), p.sim_parameters.sc);}
    }
  return comp;
}

double calc_survivability_indiv(const indiv& m, const double comp, const kewe_parameters& p)
{
  return 1.0 - (comp / (p.sim_parameters.popsize * 2))
         / (gauss(m.get_eco_trait(), p.sim_parameters.sk));
         //* gauss(m.get_male_trait(),p.sim_parameters.sq));
}

double calc_attractiveness(
    const indiv& mother,
    const indiv& father,
    const kewe_parameters& parameters
    )
{
  return gauss((mother.get_fem_pref() - father.get_male_trait()), parameters.sim_parameters.sm)
       * gauss(mother.get_eco_trait() - father.get_eco_trait(), parameters.sim_parameters.se);
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
    /// Pick 2 parents
    std::vector<double> pop_comp(static_cast<int>(pop.size()), 0.0);
    std::vector<double> pop_surv(static_cast<int>(pop.size()), 0.0);
    calc_pop_comp(pop, parameters, pop_comp);
    double surv = calc_and_set_survivability(pop, pop_comp, parameters, pop_surv);


    unsigned int m = pick_individual(pop_surv, surv, gen);
    unsigned int f;
    do {f = pick_individual(pop_surv, surv, gen);}
    while (f == m);

/*
           ///Competition
      double comp_m = calc_competition(m, pop, parameters);
      double comp_f = calc_competition(f, pop, parameters);

      /// If fitness parents is high enough, mate
      if (fitness_high_enough(pop[m], comp_m, pop[f], comp_f, parameters, gen))
        {*/
    assert(m < static_cast<unsigned int>(pop.size()));
    assert(f < static_cast<unsigned int>(pop.size()));
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

  return nextPopulation;
}

unsigned int pick_individual(
        const std::vector<double>& pop_surv,
        const double surv,
        std::mt19937& gen
    )
{
  std::uniform_real_distribution<> dis(0,surv);

  double surv_i = dis(gen);
  for(int i = 0; i < static_cast<int>(pop_surv.size()); ++i)
    {
      assert(i >= 0);
      assert(i < static_cast<int>(pop_surv.size()));
      if (surv_i <= pop_surv[i])
        return i;
    }
  throw std::invalid_argument("Could not pick an individual.");
}

double calc_and_set_survivability(
    const std::vector<indiv>& pop,
    const std::vector<double>& pop_comp,
    const kewe_parameters& parameters,
    std::vector<double>& pop_surv
    )
{
  double surv{0.0};
  for(int i = 0; i < static_cast<int>(pop_surv.size()); ++i)
  {
      assert(i >= 0);
      assert(i < static_cast<int>(pop_comp.size()));
      assert(i < static_cast<int>(pop.size()));
      assert(i < static_cast<int>(pop_surv.size()));
      surv += calc_survivability_indiv(pop[i], pop_comp[i], parameters);

      pop_surv[i] = surv;
  }

  return surv;
}

void calc_pop_comp(
    const std::vector<indiv>& pop,
    const kewe_parameters& parameters,
    std::vector<double>& pop_comp
    )
{
  for(int i = 0; i < static_cast<int>(pop_comp.size()); ++i)
    {
      assert(i >= 0);
      assert(i < static_cast<int>(pop_comp.size()));
      pop_comp[i] = calc_competition(i, pop, parameters);
    }
}


