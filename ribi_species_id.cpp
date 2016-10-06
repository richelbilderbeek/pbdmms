#include "ribi_species_id.h"
#include <iostream>

int ribi::species_id::sm_next_id = 0;

ribi::species_id::species_id()
  : m_id{sm_next_id}
{
  ++sm_next_id;
}

std::ostream& ribi::operator<<(std::ostream& os, const species_id id) noexcept
{
  os << id.get_id();
  return os;
}

bool ribi::operator==(const species_id& lhs, const species_id& rhs) noexcept
{
  return lhs.get_id() == rhs.get_id();
}

bool ribi::operator!=(const species_id& lhs, const species_id& rhs) noexcept
{
  return !(lhs == rhs);
}
