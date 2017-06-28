#include "phyg_species_id.h"

#include <cassert>
#include <iostream>

phyg::species_id::species_id(const int i)
  : m_id{i}
{

}

phyg::species_id phyg::create_new_species_id() noexcept
{
  static int sm_next_id{1};
  assert(sm_next_id != 0); //This is the null species ID
  return species_id(sm_next_id++);
}

phyg::species_id phyg::create_null_species_id() noexcept
{
  return species_id(0);
}

std::ostream& phyg::operator<<(std::ostream& os, const species_id id) noexcept
{
  os << id.get_id();
  return os;
}

bool phyg::operator==(const species_id lhs, const species_id rhs) noexcept
{
  return lhs.get_id() == rhs.get_id();
}

bool phyg::operator!=(const species_id lhs, const species_id rhs) noexcept
{
  return !(lhs == rhs);
}

bool phyg::operator<(const species_id lhs, const species_id rhs) noexcept
{
  return lhs.get_id() < rhs.get_id();
}

bool phyg::operator>(const species_id lhs, const species_id rhs) noexcept
{
  return lhs.get_id() > rhs.get_id();
}
