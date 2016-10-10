#include "jobo_parameters.h"
#include <iostream>
#include <stdexcept>

jobo_parameters::jobo_parameters(
  const int n_loci, const int population_size
  ): m_n_loci{n_loci}, m_population_size{population_size}
{
  if (n_loci < 0)
  {
    throw std::invalid_argument("n_loci must be positive");
  }
  if (population_size < 0)
  {
    throw std::invalid_argument("population_size must be positive");
  }
}

bool operator==(const jobo_parameters& lhs, const jobo_parameters& rhs)
{
  return lhs.get_n_loci() == rhs.get_n_loci();
  return lhs.get_population_size() == rhs.get_population_size();
}

bool operator!=(const jobo_parameters& lhs, const jobo_parameters& rhs)
{
  return !(lhs == rhs);
}

