#include "elly_n_species.h"

#include <stdexcept>

elly::n_species::n_species(const int n)
  : m_n{n}
{
  if (m_n < 0)
  {
    throw std::invalid_argument("Number of species is zero or positive");
  }
}

bool elly::operator==(const n_species lhs, const n_species rhs) noexcept
{
  return lhs.get() == rhs.get();
}

bool elly::operator<(const n_species lhs, const n_species rhs) noexcept
{
  return lhs.get() < rhs.get();
}

bool elly::operator<=(const n_species lhs, const n_species rhs) noexcept
{
  return lhs < rhs || lhs == rhs;
}
