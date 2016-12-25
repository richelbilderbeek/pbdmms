#include "kewe_individuals.h"

#include <algorithm>
#include <cassert>

#include "kewe_parameters.h"
#include "kewe_ses.h"

std::vector<double> kewe::calc_competitivenesses(
  const individuals& pop,
  const simulation_parameters& p
)
{
  std::vector<double> cs; //competitivenesses
  cs.reserve(pop.size());

  const int sz{static_cast<int>(pop.size())};
  for(int i = 0; i != sz; ++i)
  {
    assert(i >= 0);
    assert(i < static_cast<int>(pop.size()));
    const double c{calc_competition(i, pop, p)}; //competitiveness
    assert(c >= 0.0);
    cs.push_back(c);
  }
  return cs;
}

std::vector<double> kewe::calc_survivabilities(
  const individuals& pop,
  const std::vector<double>& pop_comp,
  const simulation_parameters& p
)
{
  assert(pop.size() == pop_comp.size());
  std::vector<double> survivabilities;
  survivabilities.reserve(pop.size());

  const int sz{static_cast<int>(pop.size())};
  double sum_surv{0.0}; //cumulative_survivability
  for(int i = 0; i != sz; ++i)
  {
    assert(i >= 0);
    assert(i < static_cast<int>(pop_comp.size()));
    assert(i < static_cast<int>(pop.size()));
    sum_surv += calc_survivability(pop[i], pop_comp[i], p);
    survivabilities.push_back(sum_surv);
  }
  assert(survivabilities.size() == pop.size());
  return survivabilities;
}

std::vector<double> kewe::calc_survivabilities(
  const individuals& pop,
  const simulation_parameters& p
)
{
  const std::vector<double> pop_comp = calc_competitivenesses(pop, p);
  return calc_survivabilities(
    pop, pop_comp, p
  );
}

kewe::individuals kewe::create_test_individuals_1() noexcept
{
  /// +---+-------------------+------------+---------+
  /// | # | female_preference | male_trait | ecotype |
  /// +---+-------------------+------------+---------+
  /// | a |       1.0         |    1.0     |   1.0   |
  /// | b |       1.0         |    2.0     |   1.0   |
  /// | c |       1.0         |    3.0     |   1.0   |
  /// +---+-------------------+------------+---------+
  return
  {
    individual(1.0, 1.0, 1.0, {1.0}, {1.0}, {1.0} ),
    individual(1.0, 1.0, 2.0, {1.0}, {1.0}, {2.0} ),
    individual(1.0, 1.0, 3.0, {1.0}, {1.0}, {3.0} )
  };
}

int kewe::randomindividual(const individuals& pop, std::mt19937& gen)
{
  assert(!pop.empty());
  std::uniform_int_distribution<> dis(0, static_cast<int>(pop.size() - 1));
  return dis(gen);

}
