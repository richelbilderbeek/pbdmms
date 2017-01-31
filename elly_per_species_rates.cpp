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

elly::per_species_rates elly::create_test_rates_1() noexcept
{
  const per_species_rate ana = per_species_rate(0.1);
  const per_species_rate clado_is = per_species_rate(0.2);
  const per_species_rate clado_main = per_species_rate(0.3);
  const per_species_rate ext_is = per_species_rate(0.4);
  const per_species_rate ext_main = per_species_rate(0.5);
  const per_species_rate mig_to_is = per_species_rate(0.6);
  return per_species_rates(
    ana,
    clado_is,
    clado_main,
    ext_is,
    ext_main,
    mig_to_is
  );
}

elly::per_species_rates elly::create_test_rates_2() noexcept
{
  const per_species_rate ana = per_species_rate(1.1);
  const per_species_rate clado_is = per_species_rate(2.2);
  const per_species_rate clado_main = per_species_rate(3.3);
  const per_species_rate ext_is = per_species_rate(4.4);
  const per_species_rate ext_main = per_species_rate(5.5);
  const per_species_rate mig_to_is = per_species_rate(6.6);
  return per_species_rates(
    ana,
    clado_is,
    clado_main,
    ext_is,
    ext_main,
    mig_to_is
  );
}

elly::per_species_rate elly::read_ana(std::istream& is)
{
  std::string s;
  is >> s;
  if (s != "ana:")
  {
    throw std::invalid_argument("Expected 'ana:'");
  }
  per_species_rate ana;
  is >> ana;
  return ana;
}

elly::per_species_rate elly::read_clado_is(std::istream& is)
{
  std::string s;
  is >> s;
  if (s != "clado_is:")
  {
    throw std::invalid_argument("Expected 'clado_is:'");
  }
  per_species_rate clado_is;
  is >> clado_is;
  return clado_is;
}

elly::per_species_rate elly::read_clado_main(std::istream& is)
{
  std::string s;
  is >> s;
  if (s != "clado_main:")
  {
    throw std::invalid_argument("Expected 'clado_main:'");
  }
  per_species_rate clado_main;
  is >> clado_main;
  return clado_main;
}

elly::per_species_rate elly::read_ext_is(std::istream& is)
{
  std::string s;
  is >> s;
  if (s != "ext_is:")
  {
    throw std::invalid_argument("Expected 'ext_is:'");
  }
  per_species_rate ext_is;
  is >> ext_is;
  return ext_is;
}

elly::per_species_rate elly::read_ext_main(std::istream& is)
{
  std::string s;
  is >> s;
  if (s != "ext_main:")
  {
    throw std::invalid_argument("Expected 'ext_main:'");
  }
  per_species_rate ext_main;
  is >> ext_main;
  return ext_main;
}

elly::per_species_rate elly::read_mig_to_is(std::istream& is)
{
  std::string s;
  is >> s;
  if (s != "mig_to_is:")
  {
    throw std::invalid_argument("Expected 'mig_to_is:'");
  }
  per_species_rate mig_to_is;
  is >> mig_to_is;
  return mig_to_is;
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
  p.m_ana = read_ana(is);
  p.m_clado_is = read_clado_is(is);
  p.m_clado_main = read_clado_main(is);
  p.m_ext_is = read_ext_is(is);
  p.m_ext_main = read_ext_main(is);
  p.m_mig_to_is = read_mig_to_is(is);
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

bool elly::operator!=(const per_species_rates& lhs, const per_species_rates& rhs) noexcept
{
  return !(lhs == rhs);
}
