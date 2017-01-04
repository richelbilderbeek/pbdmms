#include "elly_result.h"

#include <iostream>
#include <sstream>

elly::result::result(const species& s)
  : m_s{s}
{

}

std::string elly::get_result_header() noexcept
{
  return "ID,PID,t_birth_main,t_birth_island,t_ext_main,t_ext_is,t_col,clade_id";
}

std::string elly::result_to_str(
  const species_id id,
  const species_id pid,
  const double t_birth_mainland,
  const double t_birth_island,
  const double t_ext_mainland,
  const double t_ext_island,
  const double t_colonization,
  const clade_id cid
)
{
  std::stringstream s;
  s
    << id << ','
    << pid << ','
    << t_birth_mainland << ','
    << t_birth_island << ','
    << t_ext_mainland << ','
    << t_ext_island << ','
    << t_colonization << ','
    << cid;
  return s.str();

}

std::ostream& elly::operator<<(std::ostream& os, const result& r) noexcept
{
  const auto& s = r.m_s;
  os << result_to_str(
      s.get_species_id(),
      s.get_parent_id(),
      get_t_birth_mainland(s),
      get_t_birth_island(s),
      get_t_ext_mainland(s),
      get_t_ext_island(s),
      get_t_colonization(s),
      s.get_clade_id());
  return os;
}

