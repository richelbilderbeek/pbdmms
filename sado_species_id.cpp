#include "sado_species_id.h"

#include <cassert>
#include <iostream>

sado::spp_id::spp_id(const int i)
  : m_id{i}
{

}

sado::spp_id sado::create_new_species_id() noexcept
{
  static int sm_next_id{1};
  assert(sm_next_id != 0); //This is the null species ID
  return spp_id(sm_next_id++);
}

sado::spp_id sado::create_null_species_id() noexcept
{
  return spp_id(0);
}

std::ostream& sado::operator<<(std::ostream& os, const spp_id id) noexcept
{
  os << id.get_id();
  return os;
}

bool sado::operator==(const spp_id lhs, const spp_id rhs) noexcept
{
  return lhs.get_id() == rhs.get_id();
}

bool sado::operator!=(const spp_id lhs, const spp_id rhs) noexcept
{
  return !(lhs == rhs);
}

bool sado::operator<(const spp_id lhs, const spp_id rhs) noexcept
{
  return lhs.get_id() < rhs.get_id();
}

bool sado::operator>(const spp_id lhs, const spp_id rhs) noexcept
{
  return lhs.get_id() > rhs.get_id();
}
