#include "jobo_parameters.h"
#include <iostream>
#include <stdexcept>

jobo::parameters::parameters(
    const int n_loci,
    const int population_size,
    const int seed,
    const double mutation_rate,
    const int duration
    )
  : m_n_loci{n_loci},
    m_population_size{population_size},
    m_seed{seed},
    m_mutation_rate{mutation_rate},
    m_duration{duration}
{
  if (n_loci < 0)
  {
    throw std::invalid_argument("n_loci must be positive");
  }
  if (population_size < 0)
  {
    throw std::invalid_argument("population_size must be positive");
  }
  if (mutation_rate < 0)
  {
    throw std::invalid_argument("mutation_rate must be positive");
  }
  if (mutation_rate > 1)
  {
    throw std::invalid_argument("mutation_rate can't be higher than 1");
  }
  if (duration < 0)
  {
    throw std::invalid_argument("duration must be positive");
  }
}

bool jobo::operator==(const parameters& lhs, const parameters& rhs) noexcept
{
  return lhs.get_n_loci() == rhs.get_n_loci()
  && lhs.get_population_size() == rhs.get_population_size();
}

bool jobo::operator!=(const parameters& lhs, const parameters& rhs) noexcept
{
  return !(lhs == rhs);
}

