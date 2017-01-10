#include "elly_carrying_capacity.h"

#include <stdexcept>
#include <iostream>

elly::carrying_capacity::carrying_capacity(const n_species n)
  : m_n{n}
{
  if (m_n.get() == n_species(0))
  {
    throw std::invalid_argument("Carrying capacity must be non-zero");
  }
}

double elly::to_double(const carrying_capacity& k) noexcept
{
  return to_double(k.get());
}

bool elly::operator==(const carrying_capacity lhs, const carrying_capacity rhs) noexcept
{
  return lhs.get() == rhs.get();
}

bool elly::operator!=(const carrying_capacity lhs, const carrying_capacity rhs) noexcept
{
  return !(lhs == rhs);
}

bool elly::operator<(const carrying_capacity lhs, const carrying_capacity rhs) noexcept
{
  return lhs.get() < rhs.get();
}

bool elly::operator>(const carrying_capacity lhs, const carrying_capacity rhs) noexcept
{
  return !(lhs <= rhs);
}

bool elly::operator<=(const carrying_capacity lhs, const carrying_capacity rhs) noexcept
{
  return lhs < rhs || lhs == rhs;
}

bool elly::operator>=(const carrying_capacity lhs, const carrying_capacity rhs) noexcept
{
  return !(lhs < rhs);
}

std::ostream& elly::operator<<(std::ostream& os, const carrying_capacity& n) noexcept
{
  os << n.get() << " species";
  return os;
}

std::istream& elly::operator>>(std::istream& is, carrying_capacity& n)
{
  int value{0};
  std::string s; //unit
  is >> value >> s;
  n = carrying_capacity(value);
  return is;
}
