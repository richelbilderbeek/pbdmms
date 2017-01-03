#include "elly_clade_id.h"

#include <cassert>
#include <iostream>

elly::clade_id::clade_id(const int id)
  : m_id{id}
{

}

elly::clade_id elly::create_new_clade_id() noexcept
{
  static int sm_next_id{1};
  assert(sm_next_id != 0); //This is the null clade ID
  return clade_id(sm_next_id++);
}

elly::clade_id elly::create_null_clade_id() noexcept
{
  return clade_id(0);
}

std::ostream& elly::operator<<(std::ostream& os, const clade_id& id) noexcept
{
  os << id.get_id();
  return os;
}

bool elly::operator==(const clade_id lhs, const clade_id rhs) noexcept
{
  return lhs.get_id() == rhs.get_id();
}

bool elly::operator!=(const clade_id lhs, const clade_id rhs) noexcept
{
  return !(lhs == rhs);
}

