#include "elly_per_species_rates.h"

#include <iostream>

elly::per_species_rates::per_species_rates(
  const per_species_rate ana,
  const per_species_rate clado_is,
  const per_species_rate clado_main,
  const per_species_rate ext_is,
  const per_species_rate ext_main,
  const per_species_rate mig_to_is
)
  : m_ana{ana},
    m_clado_is{clado_is},
    m_clado_main{clado_main},
    m_ext_is{ext_is},
    m_ext_main{ext_main},
    m_mig_to_is{mig_to_is}
{

}

std::ostream& elly::operator<<(std::ostream& os, const per_species_rates& p) noexcept
{
  os
    << "ana: "        << p.m_ana << '\n'
    << "clado_is: "   << p.m_clado_is << '\n'
    << "clado_main: " << p.m_clado_main << '\n'
    << "ext_is: "     << p.m_ext_is << '\n'
    << "ext_main: "   << p.m_ext_main << '\n'
    << "mig_to_is: "  << p.m_mig_to_is
  ;
  return os;
}

std::istream& elly::operator>>(std::istream& is, per_species_rates& p)
{
  std::string s; //Used to write titles to
  is
    >> s >> p.m_ana
    >> s >> p.m_clado_is
    >> s >> p.m_clado_main
    >> s >> p.m_ext_is
    >> s >> p.m_ext_main
    >> s >> p.m_mig_to_is
  ;
  return is;
}

bool elly::operator==(const per_species_rates& lhs, const per_species_rates& rhs) noexcept
{
  return
       lhs.m_ana == rhs.m_ana
    && lhs.m_clado_is == rhs.m_clado_is
    && lhs.m_clado_main == rhs.m_clado_main
    && lhs.m_ext_is == rhs.m_ext_is
    && lhs.m_ext_main == rhs.m_ext_main
    && lhs.m_mig_to_is == rhs.m_mig_to_is
  ;
}
