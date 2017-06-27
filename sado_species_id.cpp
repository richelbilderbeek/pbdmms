#include "sado_species_id.h"

#include <cassert>
#include <iostream>

sado::species_id::species_id(const int i)
  : m_id{i}
{

}

sado::species_id sado::create_new_species_id() noexcept
{
  static int sm_next_id{1};
  assert(sm_next_id != 0); //This is the null species ID
  return species_id(sm_next_id++);
}

sado::species_id sado::create_null_species_id() noexcept
{
  return species_id(0);
}

std::ostream& sado::operator<<(std::ostream& os, const species_id id) noexcept
{
  os << id.get_id();
  return os;
}

bool sado::operator==(const species_id lhs, const species_id rhs) noexcept
{
  return lhs.get_id() == rhs.get_id();
}

bool sado::operator!=(const species_id lhs, const species_id rhs) noexcept
{
  return !(lhs == rhs);
}

bool sado::operator<(const species_id lhs, const species_id rhs) noexcept
{
  return lhs.get_id() < rhs.get_id();
}

bool sado::operator>(const species_id lhs, const species_id rhs) noexcept
{
  return lhs.get_id() > rhs.get_id();
}
