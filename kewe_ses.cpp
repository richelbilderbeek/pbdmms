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
#include "kewe_ses.h"
#include "kewe_helper.h"
#include "kewe_simulation.h"
#include "kewe_attractiveness.h"

bool kewe::attractive_enough(
    const individual& m,
    const individual& f,
    const simulation_parameters& p,
    std::mt19937& gen
    )
{
  std::uniform_real_distribution<> dis(0.0, 1.0);
  return dis(gen) < calc_attractiveness(
    m,
    f,
    p.get_gauss_mate_spec_mate(),
    p.get_gauss_mate_spec_eco()
  );
}

bool kewe::attractive_enough(
  const individual& m,
  const individual& f,
  const ribi::gausser& gauss_mate_spec_mate,
  const ribi::gausser& gauss_mate_spec_eco,
  std::mt19937& gen
)
{
  std::uniform_real_distribution<> dis(0.0, 1.0);
  return dis(gen) < calc_attractiveness(m, f, gauss_mate_spec_mate, gauss_mate_spec_eco);
}

bool kewe::fitness_high_enough(
    const individual& i,
    const double comp_i,
    const individual& j,
    const double comp_j,
    const simulation_parameters& parameters,
    std::mt19937& gen
)
{
  std::uniform_real_distribution<> dis(0, 1);

  return dis(gen) < calc_survivability(i, comp_i, parameters)
      && dis(gen) < calc_survivability(j, comp_j, parameters);
}

double kewe::calc_competition(
  const double a,
  const double b,
  const double sc
)
{
  assert(sc > 0.0);
  return gauss(a - b, sc);
}

double kewe::calc_competition(
  const double a,
  const double b,
  const ribi::gausser& gauss_sc
) noexcept
{
  return gauss_sc(a - b);
}

double kewe::calc_competition(
    const unsigned int i,
    const individuals& pop,
    const simulation_parameters& p
    )
{
  assert(i < static_cast<unsigned int>(pop.size()));

  //Just sum over all individuals. Because the individual competes
  //with itself, 1.0 (the competition with itself) is superflously added.
  //This is solved by setting the starting competition value to -1.0

  //Proof that the competition to itself is 1.0
  assert(
    calc_competition(
      pop.back().get_eco_trait(),
      pop.back().get_eco_trait(),
      p.get_gauss_eco_res_util_width()
    ) == 1.0
  );

  //As explained above
  double comp{-1.0};
  for (int j = 0; j < p.popsize; ++j)
  {
    const double a{pop[i].get_eco_trait()};
    const double b{pop[j].get_eco_trait()};
    const ribi::gausser& sc{p.get_gauss_eco_res_util_width()};
    assert(calc_competition(a, a, sc) == 1.0);
    assert(j < static_cast<int>(pop.size()));
    comp += calc_competition(a, b, sc);
  }
  return comp;
}

double kewe::calc_mortality(
  const double ecological_trait,
  const double eco_distr_width,
  const double comp_intensity, //competition_intensity
  const int population_size
)
{
  assert(population_size > 0);
  assert(eco_distr_width > 0.0);
  assert(comp_intensity >= 0.0);
  //No need to use gamma, as we use a contant population size
  const double gamma{
    1.0 / (2.0 * static_cast<double>(population_size))
  };
  //RJCB: Brackets OK?

  const double m {
      (gamma * comp_intensity)
    / (gauss(ecological_trait, eco_distr_width))
  };
  assert(m >= 0.0);
  #ifdef FIX_ISSUE_146
  if (m > 1.0)
  {
    std::cerr << "#146\n";
  }
  #endif
  return std::min(1.0, m); //#146
}

double kewe::calc_survivability(
  const double ecological_trait,
  const double eco_distr_width,
  const double comp_intensity, //competition_intensity
  const int population_size
)
{
  assert(population_size > 0);
  assert(eco_distr_width > 0.0);
  assert(comp_intensity >= 0.0);
  const double m {
    calc_mortality(ecological_trait, eco_distr_width, comp_intensity, population_size)
  };
  const double s {1.0 - m};
  assert(s >= 0.0);
  assert(s <= 1.0);
  return s;
}


double kewe::calc_survivability(
  const individual& m,
  const double comp_intensity, //competition_intensity
  const simulation_parameters& p
)
{
  const double ecological_trait{m.get_eco_trait()};
  const double eco_distr_width{
    p.get_eco_res_distribution_width()
  }; //ecological_distribution_width
  const int population_size{p.popsize};
  return calc_survivability(
    ecological_trait,
    eco_distr_width,
    comp_intensity,
    population_size
  );
}

std::vector<kewe::individual> kewe::create_initial_population(
  const simulation_parameters& p,
  std::mt19937& gen
)
{
  individuals pop;
  const int n{p.popsize};
  pop.reserve(n);
  for (int i=0; i!=n; ++i)
  {
    pop.push_back(individual(p, gen));
  }
  return pop;
}

std::vector<kewe::individual> kewe::create_next_generation(
  const simulation_parameters& p,
  const individuals& pop,
  std::mt19937& gen
)
{
  const ribi::gausser& gauss_mate_spec_mate = p.get_gauss_mate_spec_mate();
  const ribi::gausser& gauss_mate_spec_eco = p.get_gauss_mate_spec_eco();

  individuals nextPopulation;
  nextPopulation.reserve(pop.size());

  const auto fitnesses = calc_survivabilities(pop, p);
  std::discrete_distribution<> d(std::begin(fitnesses), std::end(fitnesses));

  while(static_cast<int>(nextPopulation.size()) < p.popsize)
  {
    // Pick 2 different parents, weighted by their fitness
    int f = d(gen);
    int m = d(gen);
    while (f == m)
    {
      f = d(gen);
    }
    assert(m < static_cast<int>(pop.size()));
    assert(f < static_cast<int>(pop.size()));
    const individual mother = pop[m];
    const individual father = pop[f];

    //Check if they want to mate
    if (attractive_enough(mother, father, gauss_mate_spec_mate, gauss_mate_spec_eco, gen))
    {
      nextPopulation.push_back(
        create_offspring(father, mother, p, gen)
      );
    }
  }

  return nextPopulation;
}

unsigned int kewe::pick_individual(
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

std::vector<double> kewe::calc_and_set_survivability(
  const individuals& pop,
  const std::vector<double>& pop_comp,
  const simulation_parameters& parameters
)
{
  const int sz{static_cast<int>(pop.size())};
  std::vector<double> pop_surv;
  pop_surv.reserve(sz);
  for(int i = 0; i < static_cast<int>(pop_surv.size()); ++i)
  {
    assert(i >= 0);
    assert(i < static_cast<int>(pop_comp.size()));
    assert(i < static_cast<int>(pop.size()));
    pop_surv.push_back(calc_survivability(pop[i], pop_comp[i], parameters));
  }
  return pop_surv;
}

std::vector<double> kewe::calc_pop_comp(
    const individuals& pop,
    const simulation_parameters& parameters
)
{
  std::vector<double> pop_comp;
  const int sz{static_cast<int>(pop.size())};
  pop_comp.reserve(sz);
  for(int i = 0; i != sz; ++i)
  {
    assert(i >= 0);
    assert(i < static_cast<int>(pop_comp.size()));
    pop_comp.push_back(calc_competition(i, pop, parameters));
  }
  return pop_comp;
}


