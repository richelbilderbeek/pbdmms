#include "kewe_attractivenesses.h"

#include <cassert>
#include "kewe_attractiveness.h"

kewe::attractivenesses kewe::calc_attractivenesses(
  const individuals& pop,
  const simulation_parameters& p
)
{
  assert(!pop.empty());
  const int sz{static_cast<int>(pop.size())};
  attractivenesses as;
  as.reserve(sz);

  for (int i = 0; i <sz; ++i)
  {
    assert(i >= 0);
    assert(i < static_cast<int>(pop.size()));
    const auto& observer = pop[i];
    std::vector<attractiveness> her_as;
    her_as.reserve(sz);

    for (int j = 0; j < sz; ++j)
    {
      assert(j >= 0);
      assert(j < static_cast<int>(pop.size()));
      const auto& observed = pop[j];
      //Individuals do not fancy themselves
      const attractiveness a{i == j ? 0.0 : calc_attractiveness(observer, observed, p)};
      her_as.push_back(a);
    }
    as.push_back(her_as);
  }
  return as;
}
