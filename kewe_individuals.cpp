#include "kewe_individuals.h"

#include <algorithm>
#include <cassert>

#include "kewe_parameters.h"
#include "kewe_SES.h"

std::vector<double> kewe::calc_competitivenesses(
  const individuals& pop,
  const parameters& p
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
  const parameters& parameters
)
{
  assert(pop.size() == pop_comp.size());
  std::vector<double> survivabilities;
  survivabilities.reserve(pop.size());

  const int sz{static_cast<int>(pop.size())};
  double cumulative_survivability{0.0};
  for(int i = 0; i != sz; ++i)
  {
    assert(i >= 0);
    assert(i < static_cast<int>(pop_comp.size()));
    assert(i < static_cast<int>(pop.size()));
    cumulative_survivability += calc_survivability(pop[i], pop_comp[i], parameters);
    survivabilities.push_back(cumulative_survivability);
  }
  assert(survivabilities.size() == pop.size());
  return survivabilities;
}

std::vector<double> kewe::calc_survivabilities(
  const individuals& pop,
  const parameters& p
)
{
  const std::vector<double> pop_comp = calc_competitivenesses(pop, p);
  return calc_survivabilities(
    pop, pop_comp, p
  );
}
