#include "elly_n_species.h"

#include <cassert>
#include <stdexcept>
#include <iostream>

elly::n_species::n_species(const int n)
  : m_n{n}
{
  if (m_n < 0)
  {
    throw std::invalid_argument("Number of species is zero or positive");
  }
}

double elly::to_double(const n_species& n) noexcept
{
  return static_cast<double>(n.get());
}

bool elly::operator==(const n_species lhs, const n_species rhs) noexcept
{
  return lhs.get() == rhs.get();
}

bool elly::operator!=(const n_species lhs, const n_species rhs) noexcept
{
  return !(lhs == rhs);
}

bool elly::operator<(const n_species lhs, const n_species rhs) noexcept
{
  return lhs.get() < rhs.get();
}

bool elly::operator>(const n_species lhs, const n_species rhs) noexcept
{
  return !(lhs <= rhs);
}

bool elly::operator<=(const n_species lhs, const n_species rhs) noexcept
{
  return lhs < rhs || lhs == rhs;
}

bool elly::operator>=(const n_species lhs, const n_species rhs) noexcept
{
  return !(lhs < rhs);
}

std::ostream& elly::operator<<(std::ostream& os, const n_species& n) noexcept
{
  os << n.get() << " species";
  return os;
}

std::istream& elly::operator>>(std::istream& is, n_species& n)
{
  int value{0};
  std::string s; //unit
  is >> value >> s;
  n = n_species(value);
  if(s != "species")
  {
    throw std::invalid_argument("number of species must have the unit 'species'");
  }
  return is;
}
