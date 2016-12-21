#include "elly_event_rates.h"

#include <cassert>
#include <vector>
#include <numeric>
#include <stdexcept>
#include <numeric>

#include "elly_location.h"
#include "elly_simulation.h"

elly::event_rates::event_rates(
  const parameters& p,
  const simulation& s
) : m_clad_mainland{calc_clad_mainland(p, s)},
    m_mainlands_ext_rate_on_mainland{calc_mainlands_ext_rate_on_mainland(p, s)},
    m_migration_to_island{calc_migration_to_island(p, s)},
    m_islands_ext_rate_on_island{calc_islands_ext_rate_on_island(p, s)},
    m_iclad{iclad},
    m_glob_spec_ext_rate_on_main{calc_glob_spec_ext_rate_on_mainland(p, s)},
    m_glob_spec_ext_rate_on_island{calc_glob_spec_ext_rate_on_island(p, s)},
    m_anagesis{calc_anagenesis(p, s)},
    m_glob_clad_island{calc_glob_clad_island(p, s)},
    m_glob_clad_mainland{calc_glob_clad_mainland(p, s)}
{

}

std::vector<double> elly::to_ratesvector(const event_rates& r) noexcept
{
  return {
    r.get_mclad(),
    r.get_mext(),
    r.get_mimm(),
    r.get_iext(),
    r.get_iclad(),
    r.get_bextm(),
    r.get_bexti(),
    r.get_bana(),
    r.get_bcladi(),
    r.get_bcladm()
  };
}

double elly::calc_anagenesis(
  const double anagenesis_rate,
  const int n_global_species
)
{
  return anagenesis_rate * static_cast<double>(n_global_species);
}


double elly::calc_anagenesis(
  const parameters& p,
  const simulation& s
)
{
  return calc_anagenesis(p.get_ana_rate(), s.count_species(location::both));
  ;
}

double elly::calc_clad_mainland(
  const parameters& p,
  const simulation& s
)
{
  const int n_main{s.count_species(location::mainland)};
  const int n_main_only{s.count_species(location::mainland_only)};
  const double f_main_only{static_cast<double>(n_main_only) / static_cast<double>(n_main)};

  //Fraction of carrying capacity reached
  const double f_k_m{static_cast<double>(n_main) / static_cast<double>(p.get_carryingcap_main())};

  return p.get_clado_rate_main() * f_main_only * (1.0 - f_k_m);
}

double elly::calc_glob_clad_island(
  const parameters& p,
  const simulation& s
)
{
  const int n_both{s.count_species(location::both)};
  const int n_species_within_clade_d{n_both}; //Stub: assume one clade

  ///Fraction of the carrying capacity of the island being filled up
  const double f_k_i{
    static_cast<double>(n_species_within_clade_d
    / static_cast<double>(p.get_carryingcap_is()))
  };

  return p.get_clado_rate_is()
    * static_cast<double>(n_both)
    * (1.0 - f_k_i)
  ;
}

double elly::calc_glob_clad_mainland(
  const parameters& p,
  const simulation& s
)
{
  return p.get_clado_rate_main() * (n_both / n_main ) * ( 1 - n_main / p.get_carryingcap_main()));

}



double elly::calc_islands_ext_rate_on_island(
  const parameters& p,
  const simulation& s
)
{
  return p.get_ext_rate_is()
    * static_cast<double>(s.count_species(location::island_only))
  ;
}

double elly::calc_mainlands_ext_rate_on_mainland(
  const parameters& p,
  const simulation& s
)
{
  return p.get_ext_rate_main()
    * static_casty<double>(s.count_species(location::mainland_only))
   ;
}

double elly::calc_migration_to_island(
  const parameters& p,
  const simulation& s
)
{
  const int n_both{s.count_species(location::both)};
  const int n_species_within_clade{n_both}; //Stub: assume one clade
  const int k_i{p.get_carryingcap_is()}; //Carrying capacity island
  ///Fraction of the carrying capacity of the island occupied
  const double f_k_i{
    static_cast<double>(n_species_within_clade)
    / static_cast<double>(k_i)
  };

  return p.get_mig_rate_to_island()
    * static_cast<double>(s.count_species(location::mainland_only))
    * (1.0 - f_k_i)
  ;

}


double elly::calc_glob_spec_ext_rate_on_mainland(
  const parameters& p,
  const simulation& s
)
{

  return p.get_ext_rate_main()
    * static_cast<double>(s.count_species(location::both))
  ;
}

double elly::calc_glob_spec_ext_rate_on_island(
  const parameters& p,
  const simulation& s
)
{
  return p.get_ext_rate_is()
    * static_cast<double>(s.count_species(location::both))
  ;
}



double elly::calc_sumrates(const event_rates& r) noexcept
{
  const auto rates = to_ratesvector(r);
  return std::accumulate(
    std::begin(rates),
    std::end(rates),
    0.0
  );
}

elly::event_rates elly::calculate_rates(
  const parameters& p,
  const simulation& s
)
{
  event_rates r;

  //Number of species

  const int n_main_only{s.count_species(location::mainland_only)};
  const int n_island_only{s.count_species(location::island_only)};
  const int n_both{s.count_species(location::both)};
  const int n_main{s.count_species(location::mainland)};
  const int n_island{s.count_species(location::island)};


  assert(n_main > 0);


  const int k_i{p.get_carryingcap_is()}; //Carrying capacity island
  const int n_species_wthin_clade{n_both}; //All species are in this clade
  const double k_i_d{static_cast<double>(k_i)}; //as double
  const double n_species_within_clade_d{static_cast<double>(n_species_wthin_clade)}; //as double
  const double mo_d{static_cast<double>(n_main_only)};
  const double io_d{static_cast<double>(n_island_only)};
  const double bo_d{static_cast<double>(n_both)};



  const double n_species_within_clade_d{static_cast<double>(n_species_wthin_clade)}; //as double
  const double k_i_d{static_cast<double>(k_i)}; //as double

  r.set_iclad(p.get_clado_rate_is() * io_d * (1.0 - (n_species_within_clade_d / k_i_d)));
  return r;
}

