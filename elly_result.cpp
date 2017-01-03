#include "elly_result.h"

#include <iostream>

elly::result::result(const species& s)
  : m_s{s}
{

}

std::string elly::get_result_header() noexcept
{
  return "ID,PID,t_birth_main,t_birth_island,t_ext_main,t_ext_is,t_col,clade_id";
}

std::ostream& elly::operator<<(std::ostream& os, const result& r) noexcept
{
  const auto& s = r.m_s;
  os
    << s.get_species_id() << ','
    << s.get_parent_id() << ','
    << get_t_birth_mainland(s) << ','
    << get_t_birth_island(s) << ','
    << get_t_ext_mainland(s) << ','
    << get_t_ext_island(s) << ','
    << get_t_colonization(s) << ','
    << s.get_clade_id()
  ;
  return os;
}
