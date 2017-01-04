#include "elly_species_id.h"

#include <cassert>
#include <iostream>
elly::species_id::species_id(const int id)
  : m_id{id}
{

}

elly::species_id elly::create_new_species_id() noexcept
{
  static int sm_next_id{1};
  assert(sm_next_id != 0); //This is the null species ID
  return species_id(sm_next_id++);
}

elly::species_id elly::create_null_species_id() noexcept
{
  return species_id(0);
}

std::ostream& elly::operator<<(std::ostream& os, const species_id id) noexcept
{
  os << id.get_id();
  return os;
}

bool elly::operator==(const species_id lhs, const species_id rhs) noexcept
{
  return lhs.get_id() == rhs.get_id();
}

bool elly::operator!=(const species_id lhs, const species_id rhs) noexcept
{
  return !(lhs == rhs);
}

bool elly::operator<(const species_id lhs, const species_id rhs) noexcept
{
  return lhs.get_id() < rhs.get_id();
}

elly::species_id elly::create_negative_of(const species_id id)
{
  assert(id.get_id() != 0);
  return species_id(-id.get_id());
}
