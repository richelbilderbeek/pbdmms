#include "elly_event_rates.h"

#include <algorithm>
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

std::vector<elly::rate> elly::to_rates(const event_rates& r) noexcept
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

std::vector<double> elly::to_doubles(const event_rates& r) noexcept
{
  const auto v = to_rates(r);
  std::vector<double> w;
  w.reserve(v.size());
  std::transform(
    std::begin(v), std::end(v), std::back_inserter(w),
    [](const rate i) { return i.get(); }
  );
  assert(v.size() == w.size());
  return w;
}

elly::rate elly::calc_anagenesis(
  const per_species_rate anagenesis_rate,
  const n_species n_global_species
)
{
  return anagenesis_rate * n_global_species;
}


elly::rate elly::calc_anagenesis(
  const parameters& p,
  const simulation& s
)
{
  return calc_anagenesis(
    p.get_ana_rate(),
    s.count_species(location::both)
  );
}

elly::rate elly::calc_clad_mainland(
  const per_species_rate clado_rate_main,
  const n_species n_mainland,
  const n_species n_mainland_only,
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
  const double f_k_m{
    static_cast<double>(n_mainland.get())
    / static_cast<double>(carrying_cap_main)
  };

  return clado_rate_main.get()
    * static_cast<double>(n_mainland_only.get())
    * (1.0 - f_k_m);
}

elly::rate elly::calc_clad_mainland(
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

elly::rate elly::calc_glob_clad_island(
  const per_species_rate clado_rate_is,
  const n_species n_species_clade,
  const int carrying_cap_is,
  const n_species n_both
)
{
  const double f_k_i{
    static_cast<double>(n_species_clade.get())
    / static_cast<double>(carrying_cap_is)
  };
  return clado_rate_is.get()
    * static_cast<double>(n_both.get())
    * (1.0 - f_k_i)
  ;
}

elly::rate elly::calc_glob_clad_island(
  const parameters& p,
  const simulation& s
)
{
  return calc_glob_clad_island(
    p.get_clado_rate_is(),
    s.count_species(location::both),
    p.get_carryingcap_is(),
    s.count_species(location::both)
  );
}

elly::rate elly::calc_glob_clad_mainland(
  const per_species_rate clado_rate_main,
  const n_species n_both,
  const n_species n_main,
  const int carrying_cap_main
)
{
  if(n_main == 0)
  {
    return 0.0;
  }
  const double f_k{
      static_cast<double>(n_main.get())
    / static_cast<double>(carrying_cap_main)
  };
  return clado_rate_main.get()
      * static_cast<double>(n_both.get())
      * (1.0 - f_k)
    ;
}

elly::rate elly::calc_glob_clad_mainland(
  const parameters& p,
  const simulation& s
)
{
  return calc_glob_clad_mainland(
    p.get_clado_rate_main(),
    s.count_species(location::both),
    s.count_species(location::mainland),
    p.get_carryingcap_main()
  );
}

elly::rate elly::calc_iclad(
  const per_species_rate rate_clad_is,
  const n_species n_island_only,
  const n_species n_species_within_clade,
  const int carrying_cap_is
)
{
  //if there are no species on island, rate is 0
  const double f_k{
    static_cast<double>(n_species_within_clade.get())
     / static_cast<double>(carrying_cap_is)
  };
  return rate_clad_is.get()
    * static_cast<double>(n_island_only.get())
    * (1.0 - f_k)
  ;
}

elly::rate elly::calc_iclad(
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

elly::rate elly::calc_islands_ext_rate_on_island(
  const per_species_rate ext_rate_is,
  const n_species n_island_only)
{
  return ext_rate_is.get()
    * static_cast<double>(n_island_only.get());
}

elly::rate elly::calc_islands_ext_rate_on_island(
  const parameters& p,
  const simulation& s
)
{
  return calc_islands_ext_rate_on_island(
    p.get_ext_rate_is(),
    s.count_species(location::island_only)
  );
}

elly::rate elly::calc_mainlands_ext_rate_on_mainland(
    const per_species_rate ext_rate_main,
    const n_species n_main_only)
{
  return ext_rate_main.get()
    * static_cast<double>(n_main_only.get())
  ;
}

elly::rate elly::calc_mainlands_ext_rate_on_mainland(
  const parameters& p,
  const simulation& s
)
{
  return calc_mainlands_ext_rate_on_mainland(
    p.get_ext_rate_main(),
    s.count_species(location::mainland_only)
  );
}

elly::rate elly::calc_migration_to_island(
    const per_species_rate mig_rate_main,
    const n_species n_species_clade,
    const int carrying_cap_is,
    const n_species n_mainland_species)
{
  const double f_k{
    static_cast<double>(n_species_clade.get())
     / static_cast<double>(carrying_cap_is)
  };
  return mig_rate_main.get()
    * static_cast<double>(n_mainland_species.get())
    * (1.0 - f_k)
  ;
}

elly::rate elly::calc_migration_to_island(
  const parameters& p,
  const simulation& s
)
{
  return calc_migration_to_island(
    p.get_mig_rate_to_island(),
    s.count_species(location::island),
    p.get_carryingcap_is(),
    s.count_species(location::mainland)
  );
}

elly::rate elly::calc_glob_spec_ext_rate_on_mainland(
  const per_species_rate ext_rate_main,
  const n_species n_both
)
{
  return ext_rate_main.get()
    * static_cast<double>(n_both.get())
  ;
}

elly::rate elly::calc_glob_spec_ext_rate_on_mainland(
  const parameters& p,
  const simulation& s
)
{
  return calc_glob_spec_ext_rate_on_mainland(
    p.get_ext_rate_main(),
    s.count_species(location::both)
  );
}

elly::rate elly::calc_glob_spec_ext_rate_on_island(
    const per_species_rate ext_rate_is,
    const n_species n_both)
{
  return ext_rate_is
    * static_cast<double>(n_both.get())
  ;
}

elly::rate elly::calc_glob_spec_ext_rate_on_island(
  const parameters& p,
  const simulation& s
)
{
  return calc_glob_spec_ext_rate_on_island(
    p.get_ext_rate_is(),
    s.count_species(location::both)
  );
}



elly::rate elly::calc_sumrates(const event_rates& r) noexcept
{
  const auto v = to_doubles(r);
  return rate(
      std::accumulate(
      std::begin(v),
      std::end(v),
      0.0
    )
  );
}

