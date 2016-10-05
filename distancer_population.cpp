#include "distancer_population.h"

std::map<sil,int> tally_sils(const population& any_population) noexcept
{
  std::map<sil,int> m;
  for (const auto individual: any_population)
  {
    const auto my_sil = individual.get_sil();
    const auto iter = m.find(my_sil);
    if (std::end(m) == iter)
    {
      m.insert(std::make_pair(my_sil, 1));
    }
    else
    {
      ++(*iter).second;
    }
  }
  assert(sum_tally(m) == static_cast<int>(any_population.size()));
  return m;
}
