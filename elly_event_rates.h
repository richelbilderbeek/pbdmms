#ifndef ELLY_RATES_H
#define ELLY_RATES_H

#include <map>
#include <vector>

#include "elly_per_species_rate.h"
#include "elly_n_species.h"
#include "elly_rate.h"
#include "elly_events.h"

namespace elly {

struct parameters;
struct simulation;

///The rates at which events take place
///These are recalculated every time an event takes place
class event_rates
{
public:
  event_rates(
    const parameters& p,
    const simulation& s
  );

  ///Rate at which anagenesis takes place, can be on any species
  rate get_ana() const noexcept {  return m_ana;  }

  ///Rate at which cladogenesis takes place with an species
  /// that occurs at both locations, and will happen on the island
  rate get_clad_glob_on_island() const noexcept {  return m_clad_glob_on_island;  }

  ///Rate at which cladogenesis takes place with an species
  /// that occurs at both locations, and will happen on the mainland
  rate get_clad_glob_on_main() const noexcept {  return m_clad_glob_on_main;  }

  ///Rate at which cladogenesis takes place with an island-only species
  rate get_clad_island_only() const noexcept {  return m_clad_island_only;  }

  ///Rate at which cladogenesis takes place with mainland-only species
  rate get_clad_main_only() const noexcept { return m_clad_main_only; }\

  ///Rate at which extinction takes place with an species
  /// that occurs at both locations, and will happen on the island
  rate get_ext_glob_on_island() const noexcept {  return m_ext_glob_on_island;  }

  ///Rate at which extinction takes place with an species
  /// that occurs at both locations, and will happen on the mainland
  rate get_ext_glob_on_main() const noexcept {  return m_ext_glob_on_main;  }

  ///Rate at which extinction takes place with an island-only species
  rate get_ext_island_only() const noexcept {  return m_ext_island_only;  }

  ///Rate at which extinction takes place with a mainland-only species
  rate get_ext_main_only() const noexcept { return m_ext_main_only;  }

  ///Rate at which mainland species migate to island
  rate get_migration_to_island() const noexcept {  return m_migration_to_island;  }

private:

  const rate m_ana;
  const rate m_clad_glob_on_island;
  const rate m_clad_glob_on_main;
  const rate m_clad_island_only;
  const rate m_clad_main_only;
  const rate m_ext_glob_on_island;
  const rate m_ext_glob_on_main;
  const rate m_ext_island_only;
  const rate m_ext_main_only;
  const rate m_migration_to_island;
};

///Anagenesis rate of all species
rate calc_anagenesis(
  const per_species_rate anagenesis_rate,
  const n_species n_global_species
);

///Anagenesis rate of all species
rate calc_anagenesis(
  const parameters& p,
  const simulation& s
);

///Cladogesis rate of species on mainland
rate calc_clad_mainland(
  const per_species_rate clado_rate_main,
  const n_species n_mainland,
  const n_species n_mainland_only,
  const int carrying_cap_main
);

///Cladogesis rate of species on mainland
rate calc_clad_mainland(
  const parameters& p,
  const simulation& s
);

///For a global species (that exist on both island and mainland),
/// the cladogenesis rate on the island
/// @param n_species_clade number of species within the clade
rate calc_glob_clad_island(
  const per_species_rate clado_rate_is,
  const n_species n_species_clade,
  const int carrying_cap_is,
  const n_species n_both
);

///For a global species (that exist on both island and mainland),
/// the cladogenesis rate on the island
rate calc_glob_clad_island(
  const parameters& p,
  const simulation& s
);

///For a global species (that exist on both island and mainland),
/// the cladogenesis rate on the mainland
rate calc_glob_clad_mainland(
  const per_species_rate clado_rate_main,
  const n_species n_both,
  const n_species n_main,
  const int carrying_cap_main
);

///For a global species (that exist on both island and mainland),
/// the cladogenesis rate on the mainland
rate calc_glob_clad_mainland(
  const parameters& p,
  const simulation& s
);

///For a global species (that exist on both island and mainland),
/// the rate at which it goes extinct on the mainland
rate calc_glob_spec_ext_rate_on_mainland(
  const per_species_rate ext_rate_main,
  const n_species n_both
);

///For a global species (that exist on both island and mainland),
/// the rate at which it goes extinct on the mainland
rate calc_glob_spec_ext_rate_on_mainland(
  const parameters& p,
  const simulation& s
);

///For a global species (that exist on both island and mainland),
/// the rate at which it goes extinct on the mainland
rate calc_glob_spec_ext_rate_on_island(
  const per_species_rate ext_rate_is,
  const n_species n_both
);

///For a global species (that exist on both island and mainland),
/// the rate at which it goes extinct on the mainland
rate calc_glob_spec_ext_rate_on_island(
  const parameters& p,
  const simulation& s
);

///cladogenesis rate of species on island
/// @param n_species_clade number of species within the clade
rate calc_iclad(
  const per_species_rate rate_clad_is,
  const n_species n_island_only,
  const n_species n_species_clade,
  const int carrying_cap_is
);

///cladogenesis rate of species on island
rate calc_iclad(
  const parameters& p,
  const simulation& s
);

///For an island-only species
/// the rate at which it goes extinct on the island
rate calc_islands_ext_rate_on_island(
  const per_species_rate ext_rate_is,
  const n_species n_island_only
);

///For an island-only species
/// the rate at which it goes extinct on the island
rate calc_islands_ext_rate_on_island(
  const parameters& p,
  const simulation& s
);

///For a mainland-only species
/// the rate at which it goes extinct on the mainland
rate calc_mainlands_ext_rate_on_mainland(
  const per_species_rate ext_rate_main,
  const n_species n_main_only
);

///For a mainland-only species
/// the rate at which it goes extinct on the mainland
rate calc_mainlands_ext_rate_on_mainland(
  const parameters& p,
  const simulation& s
);

///Migration rate from mainland to to island
/// @param n_species_clade number of species within the clade
rate calc_migration_to_island(
  const per_species_rate mig_rate_main,
  const n_species n_species_clade,
  const int carrying_cap_is,
  const n_species n_mainland_species
);

///Migration rate from mainland to to island
rate calc_migration_to_island(
  const parameters& p,
  const simulation& s
);

/// all rates stored in a vector of rate objects
std::map<event, rate> collect_rates(const event_rates& r) noexcept;

/// all rates stored in a vector of doubles
std::vector<double> to_doubles(const event_rates& r) noexcept;

//sumrates: sum of all rates
rate calc_sumrates(const event_rates& r) noexcept;

} //~namespace elly

#endif // ELLY_RATES_H
