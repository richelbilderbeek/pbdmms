#include "elly_parameters.h"

#include <cassert>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <cstdlib>


elly::parameters::parameters( //!OCLINT cannot use less parameters
  const per_species_rate rate_clado_is,
  const per_species_rate rate_clado_main,
  const per_species_rate rate_ana,
  const per_species_rate rate_ext_is,
  const per_species_rate rate_ext_main,
  const per_species_rate rate_mig_to_is,
  const carrying_capacity carryingcap_is,
  const carrying_capacity carryingcap_main,
  const int rng_seed,
  const int init_n_main_cls,
  const int init_n_main_sps,
  const double crown_age
)
  : m_rate_clado_is{rate_clado_is},
    m_rate_clado_main{rate_clado_main},
    m_rate_ana{rate_ana},
    m_rate_ext_is{rate_ext_is},
    m_rate_ext_main{rate_ext_main},
    m_rate_mig_to_is{rate_mig_to_is},
    m_carryingcap_is{carryingcap_is},
    m_carryingcap_main{carryingcap_main},
    m_rng_seed{rng_seed},
    m_init_n_main_cls{init_n_main_cls},
    m_init_n_main_sps{init_n_main_sps},
    m_crown_age{crown_age}
{
  if (m_init_n_main_cls <= 0)
  {
    throw std::invalid_argument(
      "Initial number of clades must be at least one"
    );
  }
  if(m_init_n_main_sps <= 0)
  {
    throw std::invalid_argument(
      "Initial number of species must be at least one"
    );
  }
  if(m_crown_age <= 0.0)
  {
    throw std::invalid_argument(
      "Crown age must be non-zero and positive"
    );
  }
  if(m_init_n_main_cls > m_init_n_main_sps)
  {
    throw std::invalid_argument(
      "Must have more species than clades"
    );
  }
  if(m_init_n_main_sps > m_carryingcap_main.get())
  {
    throw std::invalid_argument(
      "Must have more species on mainland than its carrying capacity"
    );
  }
}

elly::parameters elly::create_profiling_parameters() noexcept
{
  const double rate_clado_main{0.20};
  const double rate_clado_is{0.20};
  const double rate_ana{0.87};
  const double rate_ext_is{0.05};
  const double rate_ext_main{0.05};
  const double rate_mig_to_is{0.01};
  const carrying_capacity carryingcap_is{30};
  const carrying_capacity carryingcap_main{1100};
  const unsigned int rng_seed{42};
  const int init_n_main_cls{10};
  const int init_n_main_sps{20};
  const double crown_age{200.0};
  return parameters (
    rate_clado_main,
    rate_clado_is,
    rate_ana,
    rate_ext_is,
    rate_ext_main,
    rate_mig_to_is,
    carryingcap_is,
    carryingcap_main,
    rng_seed,
    init_n_main_cls,
    init_n_main_sps,
    crown_age
  );
}


elly::parameters elly::create_parameters_set1() noexcept
{
  const double rate_clado_main{0.20};
  const double rate_clado_is{0.20};
  const double rate_ana{0.87};
  const double rate_ext_is{0.09};
  const double rate_ext_main{0.09};
  const double rate_mig_to_is{0.002};
  const carrying_capacity carryingcap_is{30};
  const carrying_capacity carryingcap_main{1100};
  const unsigned int rng_seed{117};
  const int init_n_main_cls{450};
  const int init_n_main_sps{900};
  const double crown_age{10.0};
  return parameters (
    rate_clado_main,
    rate_clado_is,
    rate_ana,
    rate_ext_is,
    rate_ext_main,
    rate_mig_to_is,
    carryingcap_is,
    carryingcap_main,
    rng_seed,
    init_n_main_cls,
    init_n_main_sps,
    crown_age
  );
}

elly::parameters elly::create_parameters_set2() noexcept
{
  const double rate_clado_main{0.2};
  const double rate_clado_is{0.2};
  const double rate_ana{0.87};
  const double rate_ext_is{0.1};
  const double rate_ext_main{0.1};
  const double rate_mig_to_is{0.02};
  const carrying_capacity carryingcap_is{30};
  const carrying_capacity carryingcap_main{10};
  const unsigned int rng_seed{385};
  const int init_n_main_cls{8};
  const int init_n_main_sps{10};
  const double crown_age{10.0};
  return parameters (
    rate_clado_main,
    rate_clado_is,
    rate_ana,
    rate_ext_is,
    rate_ext_main,
    rate_mig_to_is,
    carryingcap_is,
    carryingcap_main,
    rng_seed,
    init_n_main_cls,
    init_n_main_sps,
    crown_age
  );
}

elly::parameters elly::create_parameters_set3() noexcept
{
  const double rate_clado_main{0.2};
  const double rate_clado_is{0.2};
  const double rate_ana{0.87};
  const double rate_ext_is{0.1};
  const double rate_ext_main{0.1};
  const double rate_mig_to_is{0.02};
  const carrying_capacity carryingcap_is{30};
  const carrying_capacity carryingcap_main{10};
  const unsigned int rng_seed{394};
  const int init_n_main_cls{6};
  const int init_n_main_sps{10};
  const double crown_age{10.0};
  return parameters (
    rate_clado_main,
    rate_clado_is,
    rate_ana,
    rate_ext_is,
    rate_ext_main,
    rate_mig_to_is,
    carryingcap_is,
    carryingcap_main,
    rng_seed,
    init_n_main_cls,
    init_n_main_sps,
    crown_age
  );
}

elly::parameters elly::load_parameters_from_file(const std::string& filename)
{
  std::ifstream f(filename);
  parameters p = create_parameters_set2(); //Irrelevant, will be overwritten
  f >> p;
  return p;
}

void elly::save(const parameters& p, const std::string& filename)
{
  std::ofstream f(filename);
  f << p;
}

bool elly::operator==(const parameters& lhs, const parameters& rhs) noexcept //!OCLINT cannot be simpler
{
  return
    lhs.get_ana_rate() == rhs.get_ana_rate()
    && lhs.get_mig_rate_to_island() == rhs.get_mig_rate_to_island()
    && lhs.get_ext_rate_is() == rhs.get_ext_rate_is()
    && lhs.get_ext_rate_main() == rhs.get_ext_rate_main()
    && lhs.get_clado_rate_is() == rhs.get_clado_rate_is()
    && lhs.get_clado_rate_main() == rhs.get_clado_rate_main()
    && lhs.get_carryingcap_is() == rhs.get_carryingcap_is()
    && lhs.get_carryingcap_main() == rhs.get_carryingcap_main()
    && lhs.get_rng_seed() == rhs.get_rng_seed()
    && lhs.get_init_n_main_cls() == rhs.get_init_n_main_cls()
    && lhs.get_init_n_main_sps() == rhs.get_init_n_main_sps()
  ;
}

bool elly::operator!=(const parameters& lhs, const parameters& rhs) noexcept
{
  return !(lhs == rhs);
}

std::ostream& elly::operator<<(std::ostream& os, const parameters& p) noexcept
{
  os
    << "carryingcap_is: " << p.m_carryingcap_is << '\n'
    << "carryingcap_main: " << p.m_carryingcap_main << '\n'
    << "crown_age: " << p.m_crown_age << '\n'
    << "init_n_main_cls: " << p.m_init_n_main_cls << '\n'
    << "init_n_main_sps: " << p.m_init_n_main_sps << '\n'
    << "rate_ana: " << p.m_rate_ana << '\n'
    << "rate_clado_is: "   << p.m_rate_clado_is << '\n'
    << "rate_clado_main: " << p.m_rate_clado_is << '\n'
    << "rate_ext_is: " << p.m_rate_ext_is << '\n'
    << "rate_ext_main: " << p.m_rate_ext_main << '\n'
    << "rate_mig_to_is: " << p.m_rate_mig_to_is << '\n'
    << "rng_seed: " << p.m_rng_seed << '\n'
  ;
  return os;
}

std::istream& elly::operator>>(std::istream& is, parameters& p)
{
  per_species_rate rate_clado_is{0.0};
  per_species_rate rate_clado_main{0.0};
  per_species_rate rate_ana{0.0};
  per_species_rate rate_ext_is{0.0};
  per_species_rate rate_ext_main{0.0};
  per_species_rate rate_mig_to_is{0.0};
  carrying_capacity carryingcap_is{1};
  carrying_capacity carryingcap_main{1};
  int rng_seed{0};
  int init_n_main_cls{0};
  int init_n_main_sps{0};
  double crown_age{0.0};

  std::string s; //Used to write titles to
  is
    >> s >> carryingcap_is
    >> s >> carryingcap_main
    >> s >> crown_age
    >> s >> init_n_main_cls
    >> s >> init_n_main_sps
    >> s >> rate_ana
    >> s >> rate_clado_is
    >> s >> rate_clado_is
    >> s >> rate_ext_is
    >> s >> rate_ext_main
    >> s >> rate_mig_to_is
    >> s >> rng_seed
  ;
  p = parameters(
    rate_clado_is,
    rate_clado_main,
    rate_ana,
    rate_ext_is,
    rate_ext_main,
    rate_mig_to_is,
    carryingcap_is,
    carryingcap_main,
    rng_seed,
    init_n_main_cls,
    init_n_main_sps,
    crown_age
  );
  return is;
}

