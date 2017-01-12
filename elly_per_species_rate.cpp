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

void elly::read_string(std::istream& is, const std::string& label)
{
  std::string s; //To write unit to
  is >> s;
  if (s != label)
  {
    std::string
    throw std::invalid_argument("Expected per_species_rate 'per'");
  }
}

std::istream& elly::operator>>(std::istream& is, per_species_rate& r)
{
  double d{0.0};
  is >> d;
  read_string(is, "per");
  read_string(is, "species");
  read_string(is, "per");
  read_string(is, "time");
  read_string(is, "unit");
  is >> s;
  if (s != "species")
  {
    throw std::invalid_argument("Expected per_species_rate 'species'");
  }
  is >> s;
  if (s != "per")
  {
    throw std::invalid_argument("Expected per_species_rate 'per'");
  }
  is >> s;
  if (s != "time")
  {
    throw std::invalid_argument("Expected per_species_rate 'time'");
  }
  is >> s;
  if (s != "unit")
  {
    throw std::invalid_argument("Expected per_species_rate 'unit'");
  }
  r = per_species_rate(d);
  return is;
}
