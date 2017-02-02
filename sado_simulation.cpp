#include "sado_simulation.h"

#include "sado_helper.h"
#include "sado_individual.h"
#include "sado_output.h"
#include "sado_population.h"
#include "sado_random.h"
#include <algorithm>
#include <boost/algorithm/string/split.hpp>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <numeric>
#include <sstream>

sado::simulation::simulation(const parameters &p)
    : m_parameters{p}, m_population{}, m_results(p), m_timestep{0}
{
  // Must first set the seed, then initialize the population
  SetSeed(p.get_seed());
  m_population = create_initial_population(m_parameters);

  create_header(p);
}

sado::population
sado::create_next_generation(const population &pop, const parameters &p)
{
  if (p.get_next_gen_method() == next_generation_method::overlapping)
  {
    return create_next_generation_overlapping(pop, p);
  }
  assert(p.get_next_gen_method() == next_generation_method::seperate);
  return create_next_generation_seperate(pop, p);
}

sado::population
sado::create_next_generation_overlapping(population pop, const parameters &p)
{
  for (int k = 0; k < static_cast<int>(pop.size()); ++k)
  {
    if (pop.empty())
    {
      return pop;
    }
    const int index{pick_random_individual_index(pop.size())};
    // Can be zero kids
    const auto kids_and_father = try_to_create_kids(pop, index, p);
    for (auto kid_and_father : kids_and_father)
    {
      const indiv& mother = pop[index];
      pop.add_indiv(kid_and_father.first, mother, kid_and_father.second);
    }
    // Always kill the mother
    kill_mother(index, pop, p);
  }
  return pop;
}

sado::population sado::create_next_generation_seperate(
    const population &pop, const parameters &p)
{
  if (pop.empty())
  {
    return pop;
  }

  population next_pop;
  while (static_cast<int>(next_pop.size()) < p.get_pop_size())
  {
    const int index{pick_random_individual_index(pop.size())};
    // Can be zero kids
    const auto kids_with_fathers = try_to_create_kids(pop, index, p);
    const auto mother = pop[index];
    for (auto kid_with_father : kids_with_fathers)
    {
      next_pop.add_indiv(kid_with_father.first, mother, kid_with_father.second);
    }
  }
  // In the last round, there may have been produced superfluous offspring
  // kill those last ones here
  assert(static_cast<int>(next_pop.size()) >= p.get_pop_size());
  // Bye bye!
  next_pop.downsize(p.get_pop_size());

  assert(static_cast<int>(next_pop.size()) == p.get_pop_size());
  return next_pop;
}
void sado::simulation::do_timestep()
{
  assert(m_parameters.get_output_freq() > 0);
  if (m_population.empty())
    return;
  if (m_timestep % m_parameters.get_output_freq() == 0)
  {
    output(m_population, m_timestep, m_parameters, m_results);
  }
  const auto next_generation = create_next_generation(m_population, m_parameters);
  assert(m_population != next_generation);
  m_population = next_generation;

  ++m_timestep;
}

void sado::simulation::run()
{
  for (; m_timestep <= m_parameters.get_end_time();)
  {
    do_timestep();
  }
}

double sado::calc_comp(
    const population &pop, const double xi, const parameters &p) noexcept
{
  return std::accumulate(
      std::begin(pop.get_population()),
      std::end(pop.get_population()),
      -1.0,
      [p, xi](double init, const indiv &i) {
        return init + p.get_gausser_sc()(xi - i.get_x());
      });
}

std::vector<std::pair<sado::indiv, sado::indiv>> sado::create_kids(
    const population &pop,
    const indiv &mother,
    const std::vector<double> &raw_as,
    const parameters &p)
{
  // Cumulative attractivenesses
  const double b{p.get_b()};
  const std::vector<double> as{get_summed(raw_as)};
  const double eta{p.get_eta()};
  const double sum_a{as.back() + eta};
  //offspring kids;
  std::vector<std::pair<sado::indiv, sado::indiv>> family;
  for (double nkid = 0.0;; nkid += 1.0)
  {
    if (Uniform() >= b - nkid)
      break;
    const double draw = Uniform() * sum_a;
    if (draw > eta)
    {
      for (int index{0};; ++index)
      {
        // There must be an individual that is attractive enough
        assert(index < static_cast<int>(pop.size()));
        if (draw <= as[index] + eta)
        {
          const indiv& father = pop[index];
          std::pair<indiv, indiv> kid_and_father;
          kid_and_father.second = father;
          kid_and_father.first = create_offspring(mother, father, p);
          family.push_back(kid_and_father);
          break;
        }
      }
    }
  }
  return family;
}

sado::population sado::create_initial_population(const parameters &p)
{
  return population(
    std::vector<indiv>(
      p.get_pop_size(),
      create_init_with_bug(p.get_x0(), p.get_p0(), p.get_q0(), p)
    )
  );
}

void sado::kill_mother(const int index, population &pop, const parameters &p)
{
  assert(index < static_cast<int>(pop.size()));
  if (p.get_erasure() == erasure_method::erase)
  {
    assert(index < static_cast<int>(pop.size()));
    pop.erase(index);
  }
  else
  {
    assert(index < static_cast<int>(pop.size()));
    std::swap(pop[index], pop.back());
    pop.pop_back();
  }
}

std::vector<std::pair<sado::indiv, sado::indiv>> sado::try_to_create_kids(
    const population &pop, const int index, const parameters &p)
{
  assert(index < static_cast<int>(pop.size()));
  const indiv mother{pop[index]};
  const double xi = mother.get_x();
  const double pi = mother.get_p();
  const double qi = mother.get_q();
  const double comp{calc_comp(pop, xi, p)};
  const double c{p.get_c()};
  if (Uniform() < (1.0 - ((comp * c) / p.get_gausser_sk()(xi))) *
                      (0.5 + (0.5 * p.get_gausser_sq()(qi))))
  {
    // The attractivenesses you have with pi and xi
    std::vector<double> as{get_attractivenesses(pop, pi, xi, p)};
    // Unattracted to yourself
    as[index] = 0.0;
    // Get kids
    return create_kids(pop, mother, as, p);
  }
  return {}; // No kids
}

std::vector<double> sado::get_attractivenesses(
    const population &pop,
    const double pi,
    const double xi,
    const parameters &p)
{
  std::vector<double> as(pop.size(), 0.0);
  int index{0};
  for (auto j = std::cbegin(pop.get_population()); j != std::cend(pop.get_population()); j++)
  {
    const double qj{j->get_q()};
    const double xj{j->get_x()};
    as[index] = p.get_gausser_sm()(pi - qj) * p.get_gausser_se()(xi - xj);
    ++index;
  }
  return as;
}
