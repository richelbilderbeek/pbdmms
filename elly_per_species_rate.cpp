#include "elly_rate.h"

#include <stdexcept>

elly::per_species_rate::per_species_rate(const double r)
  : m_r{r}
{
  if (m_r < 0.0)
  {
    throw std::invalid_argument("rate cannot be negative");
  }
}

bool elly::operator==(const per_species_rate& lhs, const per_species_rate& rhs) noexcept
{
  return lhs.get() == rhs.get();
}
