#include "phyg_id.h"

#include <cassert>
#include <iostream>

phyg::id::id(const int i)
  : m_id{i}
{

}

phyg::id phyg::create_new_id() noexcept
{
  static int sm_next_id{1};
  assert(sm_next_id != 0); //This is the null species ID
  return id(sm_next_id++);
}

phyg::id phyg::create_null_id() noexcept
{
  return id(0);
}

std::ostream& phyg::operator<<(std::ostream& os, const id id) noexcept
{
  os << id.get_id();
  return os;
}

bool phyg::operator==(const id lhs, const id rhs) noexcept
{
  return lhs.get_id() == rhs.get_id();
}

bool phyg::operator!=(const id lhs, const id rhs) noexcept
{
  return !(lhs == rhs);
}

bool phyg::operator<(const id lhs, const id rhs) noexcept
{
  return lhs.get_id() < rhs.get_id();
}

bool phyg::operator>(const id lhs, const id rhs) noexcept
{
  return lhs.get_id() > rhs.get_id();
}

