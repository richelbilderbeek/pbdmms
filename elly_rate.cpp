#include "elly_rate.h"

#include <stdexcept>

elly::rate::rate(const double r)
  : m_r{r}
{
  if (m_r < 0.0)
  {
    throw std::invalid_argument("rate cannot be negative");
  }
}

bool elly::operator==(const rate lhs, const rate rhs) noexcept
{
  return lhs.get() == rhs.get();
}
bool elly::operator<(const rate lhs, const rate rhs) noexcept
{
  return lhs.get() < rhs.get();
}
bool elly::operator<=(const rate lhs, const rate rhs) noexcept
{
  return lhs < rhs || lhs == rhs;
}

elly::rate elly::operator*(const per_species_rate psr, const n_species n) noexcept
{
  return rate(
    psr.get() * static_cast<double>(n.get())
  );
}

