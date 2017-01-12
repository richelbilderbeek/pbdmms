#include "elly_parameters.h"

#include <cassert>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <cstdlib>


elly::parameters::parameters(
  const per_species_rates& rates,
  const carrying_capacity carryingcap_is,
  const carrying_capacity carryingcap_main,
  const int rng_seed,
  const int init_n_main_cls,
  const int init_n_main_sps,
  const double crown_age
)
  :
    m_carryingcap_is{carryingcap_is},
    m_carryingcap_main{carryingcap_main},
    m_crown_age{crown_age},
    m_init_n_main_cls{init_n_main_cls},
    m_init_n_main_sps{init_n_main_sps},
    m_rates{rates},
    m_rng_seed{rng_seed}
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
  return create_parameters_set2();
}


elly::parameters elly::create_parameters_set1() noexcept
{
  const per_species_rate ana{0.87};
  const per_species_rate clado_is{0.20};
  const per_species_rate clado_main{0.20};
  const per_species_rate ext_is{0.09};
  const per_species_rate ext_main{0.09};
  const per_species_rate mig_to_is{0.002};
  const per_species_rates rates(
    ana,
    clado_is,
    clado_main,
    ext_is,
    ext_main,
    mig_to_is
  );
  const carrying_capacity carryingcap_is{30};
  const carrying_capacity carryingcap_main{1100};
  const unsigned int rng_seed{117};
  const int init_n_main_cls{450};
  const int init_n_main_sps{900};
  const double crown_age{10.0};
  return parameters (
    rates,
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
  const per_species_rate ana{0.87};
  const per_species_rate clado_is{0.2};
  const per_species_rate clado_main{0.2};
  const per_species_rate ext_is{0.1};
  const per_species_rate ext_main{0.1};
  const per_species_rate mig_to_is{0.02};
  const per_species_rates rates(
    ana,
    clado_is,
    clado_main,
    ext_is,
    ext_main,
    mig_to_is
  );
  const carrying_capacity carryingcap_is{30};
  const carrying_capacity carryingcap_main{10};
  const unsigned int rng_seed{385};
  const int init_n_main_cls{8};
  const int init_n_main_sps{10};
  const double crown_age{10.0};
  return parameters (
    rates,
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
  const per_species_rate ana{0.87};
  const per_species_rate clado_is{0.2};
  const per_species_rate clado_main{0.2};
  const per_species_rate ext_is{0.1};
  const per_species_rate ext_main{0.1};
  const per_species_rate mig_to_is{0.02};
  const per_species_rates rates(
    ana,
    clado_is,
    clado_main,
    ext_is,
    ext_main,
    mig_to_is
  );

  const carrying_capacity carryingcap_is{30};
  const carrying_capacity carryingcap_main{10};
  const unsigned int rng_seed{394};
  const int init_n_main_cls{6};
  const int init_n_main_sps{10};
  const double crown_age{10.0};
  return parameters (
    rates,
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

elly::carrying_capacity elly::read_carrying_cap_is(std::istream& is)
{
  std::string s;
  is >> s;
  if(s != "carryingcap_is:")
  {
    throw std::invalid_argument("Expected 'carryingcap_is:'");
  }
  carrying_capacity carryingcap_is{1};
  is >> carryingcap_is;
  return carryingcap_is;
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
    && lhs.get_mig_to_island() == rhs.get_mig_to_island()
    && lhs.get_ext_is() == rhs.get_ext_is()
    && lhs.get_ext_main() == rhs.get_ext_main()
    && lhs.get_clado_is() == rhs.get_clado_is()
    && lhs.get_clado_main() == rhs.get_clado_main()
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
    << p.m_rates << '\n'
    << "rng_seed: " << p.m_rng_seed << '\n'
  ;
  return os;
}

std::istream& elly::operator>>(std::istream& is, parameters& p)
{
  per_species_rates rates;
  //carrying_capacity carryingcap_is{1};
  carrying_capacity carryingcap_main{1};
  int rng_seed{0};
  int init_n_main_cls{0};
  int init_n_main_sps{0};
  double crown_age{0.0};

  const carrying_capacity carryingcap_is{
    read_carrying_cap_is(is)
  };

  std::string s; //Used to write titles to
  is >> s;
  if(s != "carryingcap_main:")
  {
    throw std::invalid_argument("Expected 'carryingcap_main:'");
  }
  is >> carryingcap_main;
  is >> s;
  if(s != "crown_age:")
  {
    throw std::invalid_argument("Expected 'crown_age:'");
  }
  is >> crown_age;
  is >> s;
  if(s != "init_n_main_cls:")
  {
    throw std::invalid_argument("Expected 'init_n_main_cls:'");
  }
  is >> init_n_main_cls;
  is >> s;
  if(s != "init_n_main_sps:")
  {
    throw std::invalid_argument("Expected 'init_n_main_sps'");
  }
  is >> init_n_main_sps;
  is >> rates;
  is >> s;
  if(s != "rng_seed:")
  {
    throw std::invalid_argument("Expected 'rng_seed:'");
  }
  is >> rng_seed;
  p = parameters(
    rates,
    carryingcap_is,
    carryingcap_main,
    rng_seed,
    init_n_main_cls,
    init_n_main_sps,
    crown_age
  );
  return is;
}

