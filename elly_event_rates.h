#ifndef ELLY_RATES_H
#define ELLY_RATES_H

#include <vector>

#include "elly_per_species_rate.h"
#include "elly_n_species.h"
#include "elly_rate.h"

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

  rate get_mclad() const noexcept { return m_clad_mainland; }
  rate get_mext() const noexcept { return m_mainlands_ext_rate_on_mainland;  }
  rate get_mimm() const noexcept {  return m_migration_to_island;  }
  rate get_iext() const noexcept {  return m_islands_ext_rate_on_island;  }
  rate get_iclad() const noexcept {  return m_iclad;  }
  rate get_bextm() const noexcept {  return m_glob_spec_ext_rate_on_main;  }
  rate get_bexti() const noexcept {  return m_glob_spec_ext_rate_on_island;  }
  rate get_bana() const noexcept {  return m_anagesis;  }
  rate get_bcladi() const noexcept {  return m_glob_clad_island;  }
  rate get_bcladm() const noexcept {  return m_glob_clad_mainland;  }
  void set_mext(const double mext);
  void set_mimm(const double mimm);
  void set_iext(const double iext);
  void set_iclad(const double iclad);
  void set_anagenesis(const double bana);
  void set_bcladi(const double bcladi);
  void set_bcladm(const double bcladm);


private:
  ///cladogesis rate of species on mainland, per million years
  rate m_clad_mainland;

  //mext: mainland species' extinction rate of all species on mainland
  rate m_mainlands_ext_rate_on_mainland;

  ///migration from mainland to to island
  rate m_migration_to_island;

  //iext: island extinction rate of all species on island
  rate m_islands_ext_rate_on_island;

  ///cladogenesis rate of species on island
  rate m_iclad;

  ///For species that exist on both island and mainland, the rate
  ///at which it goes extinct on the mainland
  rate m_glob_spec_ext_rate_on_main;

  ///For species that exist on both island and mainland, the rate
  ///at which it goes extinct on the island
  rate m_glob_spec_ext_rate_on_island;

  //bana: anagenesis rate of all species
  rate m_anagesis;

  //bcladi: island cladogenesis rate of global species
  rate m_glob_clad_island;

  //bcladm: mainland cladogenesis rate of global species
  rate m_glob_clad_mainland;
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
std::vector<rate> to_rates(const event_rates& r) noexcept;

/// all rates stored in a vector of doubles
std::vector<double> to_doubles(const event_rates& r) noexcept;

//sumrates: sum of all rates
rate calc_sumrates(const event_rates& r) noexcept;

} //~namespace elly

#endif // ELLY_RATES_H
