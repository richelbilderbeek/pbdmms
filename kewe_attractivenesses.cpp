#include "kewe_attractivenesses.h"

#include <cassert>
#include "kewe_attractiveness.h"
#include "kewe_helper.h"
#include "kewe_simulation_parameters.h"

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

      const ribi::gausser& gauss_mate_spec_mate = p.get_gauss_mate_spec_mate();
      const ribi::gausser& gauss_mate_spec_eco = p.get_gauss_mate_spec_eco();

      //Individuals do not fancy themselves
      const attractiveness a{
        i == j
        ? 0.0
        : calc_attractiveness(
          observer,
          observed,
          gauss_mate_spec_mate,
          gauss_mate_spec_eco
          )
      };
      her_as.push_back(a);
    }
    as.push_back(her_as);
  }
  return as;
}

bool kewe::is_valid(const attractivenesses& as) noexcept
{
  return !as.empty()
    && is_square(as)
    && has_diagonal_of_zeroes(as) //Individuals do not find themselves attractive
  ;
}
