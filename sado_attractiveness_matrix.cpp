#include "sado_attractiveness_matrix.h"

#include "sado_attractiveness_vector.h"
#include "sado_helper.h"
#include "sado_population.h"
#include <gsl/gsl>

sado::attractiveness_matrix sado::create_attractiveness_matrix(
  const population& pop, const parameters& p)
{
  attractiveness_matrix as;
  const auto sz = pop.size();
  as.reserve(sz);
  for (auto i = 0; i != sz; ++i)
  {
    as.push_back(get_attractivenesses(pop, pop[i].get_p(),pop[i].get_x(), p));
  }
  set_diagonal_of_zeroes(as);
  Ensures(is_valid(as));
}

bool sado::is_valid(const attractiveness_matrix& as) noexcept
{
  return !as.empty()
    && is_square(as)
    && has_diagonal_of_zeroes(as) //Individuals do not find themselves attractive
  ;
}
