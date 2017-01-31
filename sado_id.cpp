#include "sado_id.h"

#include <cassert>
#include <iostream>
sado::id::id(const int i)
  : m_id{i}
{

}

sado::id sado::create_new_id() noexcept
{
  static int sm_next_id{1};
  assert(sm_next_id != 0); //This is the null species ID
  return id(sm_next_id++);
}

sado::id sado::create_null_id() noexcept
{
  return id(0);
}

std::ostream& sado::operator<<(std::ostream& os, const id id) noexcept
{
  os << id.get_id();
  return os;
}

bool sado::operator==(const id lhs, const id rhs) noexcept
{
  return lhs.get_id() == rhs.get_id();
}

bool sado::operator!=(const id lhs, const id rhs) noexcept
{
  return !(lhs == rhs);
}

bool sado::operator<(const id lhs, const id rhs) noexcept
{
  return lhs.get_id() < rhs.get_id();
}
