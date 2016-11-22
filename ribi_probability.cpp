#include "ribi_probability.h"

#include <iostream>
#include <stdexcept>

ribi::probability::probability(const double p)
  : m_p(p)
{
  if (p < 0.0)
  {
    throw std::invalid_argument("A probability is at least 0.0");
  }
  if (p > 1.0)
  {
    throw std::invalid_argument("A probability is at most 1.0");
  }
}

std::ostream& ribi::operator<<(std::ostream& os, const probability p) noexcept
{
  os << p.get();
  return os;
}

std::istream& ribi::operator>>(std::istream& is, probability& p)
{
  double x{0.0};
  is >> x;
  p = probability(x);
  return is;
}

bool ribi::operator==(const probability lhs, const probability rhs) noexcept
{
  return lhs.get() == rhs.get();
}

bool ribi::operator!=(const probability lhs, const probability rhs) noexcept
{
  return !(lhs == rhs);
}
