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
    m_iclad{calc_iclad(p, s)},
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
  const double clado_rate_main,
  const int n_mainland,
  const int n_mainland_only,
  const int carrying_cap_main
)
{
  assert(n_mainland_only <= n_mainland);
  //clad_main: rate * n_main_only *(1- n_main / carryingcap),
  //dus in de test 0.02*200*(1- (350/500)) =1.2

  //preventing dividing by zero, rate equals 0 with no species
  if(n_mainland == 0.0)
    return 0.0;

  //Fraction of carrying capacity reached
  const double f_k_m{static_cast<double>(n_mainland) / static_cast<double>(carrying_cap_main)};

  return clado_rate_main * n_mainland_only * (1.0 - f_k_m);
}

double elly::calc_clad_mainland(
  const parameters& p,
  const simulation& s
)
{


  return calc_clad_mainland(
    p.get_clado_rate_main(),
    s.count_species(location::mainland),
    s.count_species(location::mainland_only),
    p.get_carryingcap_main()
  );
}

double elly::calc_glob_clad_island(
  const double clado_rate_is,
  const int n_species_within_clade_d,
  const int carrying_cap_is,
  const int n_both
)
{
  const double f_k_i{
    static_cast<double>(n_species_within_clade_d
    / static_cast<double>(carrying_cap_is))
  };
  return clado_rate_is
    * static_cast<double>(n_both)
    * (1.0 - f_k_i);
}

double elly::calc_glob_clad_island(
  const parameters& p,
  const simulation& s
)
{
  return calc_glob_clad_island(
        p.get_clado_rate_is(),
        s.count_species(location::both),
        p.get_carryingcap_is(),
        s.count_species(location::both));
}

double elly::calc_glob_clad_mainland(const double clado_rate_main,
                                     const int n_both,
                                     const int n_main,
                                     const int carrying_cap_main)
{
  if(n_main == 0)
    return 0.0;

  return clado_rate_main
      * static_cast<double>(n_both)
      * (1.0 - (static_cast<double>(n_main) / static_cast<double>(carrying_cap_main)))
    ;
}

double elly::calc_glob_clad_mainland(
  const parameters& p,
  const simulation& s
)
{

  return calc_glob_clad_mainland(p.get_clado_rate_main(),
         s.count_species(location::both),
         s.count_species(location::mainland),
         p.get_carryingcap_main()
         );
}

double elly::calc_iclad(
  const double rate_clad_is,
  const int n_island_only,
  const int n_species_within_clade,
  const int carrying_cap_is
)
{
  //if there are no species on island, rate is 0
  const double f_k{
    static_cast<double>(n_species_within_clade)
     / static_cast<double>(carrying_cap_is)
  };
  return rate_clad_is
    * n_island_only
    * ( 1.0 - f_k);
}

double elly::calc_iclad(
  const parameters& p,
  const simulation& s
)
{
  return calc_iclad(
    p.get_clado_rate_is(),
    s.count_species(location::island_only),
    s.count_species(location::island), //all species on island are in the same clade
    p.get_carryingcap_is());
}

double elly::calc_islands_ext_rate_on_island(
    const double ext_rate_is,
    const int n_island_only)
{
  return ext_rate_is *
    static_cast<double>(n_island_only);
}

double elly::calc_islands_ext_rate_on_island(
  const parameters& p,
  const simulation& s
)
{
  return calc_islands_ext_rate_on_island(p.get_ext_rate_is(),
                                         s.count_species(location::island_only));
}

double elly::calc_mainlands_ext_rate_on_mainland(
    const double ext_rate_main,
    const int n_main_only)
{
  return ext_rate_main *
      n_main_only;
}

double elly::calc_mainlands_ext_rate_on_mainland(
  const parameters& p,
  const simulation& s
)
{
  return calc_mainlands_ext_rate_on_mainland(p.get_ext_rate_main(),
                                             s.count_species(location::mainland_only));
}

double elly::calc_migration_to_island(
    const double mig_rate_main,
    const int n_species_in_clade_d,
    const int carrying_cap_is,
    const int n_mainland_species)
{
  return mig_rate_main *
      n_mainland_species *
      (1.0 - n_species_in_clade_d / carrying_cap_is);
}

double elly::calc_migration_to_island(
  const parameters& p,
  const simulation& s
)
{
  return calc_migration_to_island(
        p.get_mig_rate_to_island(),
        s.count_species(location::island),
        p.get_carryingcap_is(),
        s.count_species(location::mainland));
}

double elly::calc_glob_spec_ext_rate_on_mainland(
    const double ext_rate_main,
    const int n_both)
{
  return ext_rate_main *
      static_cast<double>(n_both);
}

double elly::calc_glob_spec_ext_rate_on_mainland(
  const parameters& p,
  const simulation& s
)
{

  return calc_glob_spec_ext_rate_on_mainland(
        p.get_ext_rate_main(),
        s.count_species(location::both));
}

double elly::calc_glob_spec_ext_rate_on_island(
    const double ext_rate_is,
    const int n_both)
{
  return ext_rate_is *
      static_cast<double>(n_both);
}

double elly::calc_glob_spec_ext_rate_on_island(
  const parameters& p,
  const simulation& s
)
{
  return calc_glob_spec_ext_rate_on_island(
        p.get_ext_rate_is(),
        s.count_species(location::both));
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

