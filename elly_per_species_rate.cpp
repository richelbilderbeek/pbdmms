#include "elly_rate.h"

#include <iostream>
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

bool elly::operator!=(const per_species_rate& lhs, const per_species_rate& rhs) noexcept
{
  return !(lhs == rhs);
}

std::ostream& elly::operator<<(std::ostream& os, const per_species_rate& r) noexcept
{
  os << r.get() << " per species per time unit";
  return os;
}
