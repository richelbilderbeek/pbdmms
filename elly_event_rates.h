#ifndef ELLY_RATES_H
#define ELLY_RATES_H

#include <vector>

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

  double get_mclad() const noexcept { return m_clad_mainland; }
  double get_mext() const noexcept { return m_mainlands_ext_rate_on_mainland;  }
  double get_mimm() const noexcept {  return m_migration_to_island;  }
  double get_iext() const noexcept {  return m_islands_ext_rate_on_island;  }
  double get_iclad() const noexcept {  return m_iclad;  }
  double get_bextm() const noexcept {  return m_glob_spec_ext_rate_on_main;  }
  double get_bexti() const noexcept {  return m_glob_spec_ext_rate_on_island;  }
  double get_bana() const noexcept {  return m_anagesis;  }
  double get_bcladi() const noexcept {  return m_glob_clad_island;  }
  double get_bcladm() const noexcept {  return m_glob_clad_mainland;  }
  void set_mext(const double mext);
  void set_mimm(const double mimm);
  void set_iext(const double iext);
  void set_iclad(const double iclad);
  void set_anagenesis(const double bana);
  void set_bcladi(const double bcladi);
  void set_bcladm(const double bcladm);


private:
  ///cladogesis rate of species on mainland, per million years
  double m_clad_mainland;

  //mext: mainland species' extinction rate of all species on mainland
  double m_mainlands_ext_rate_on_mainland;

  ///migration from mainland to to island
  double m_migration_to_island;

  //iext: island extinction rate of all species on island
  double m_islands_ext_rate_on_island;

  //iclad: island cladogenesis rate of all species on island
  double m_iclad;

  ///For species that exist on both island and mainland, the rate
  ///at which it goes extinct on the mainland
  double m_glob_spec_ext_rate_on_main;

  ///For species that exist on both island and mainland, the rate
  ///at which it goes extinct on the island
  double m_glob_spec_ext_rate_on_island;

  //bana: anagenesis rate of all species
  double m_anagesis;

  //bcladi: island cladogenesis rate of global species
  double m_glob_clad_island;

  //bcladm: mainland cladogenesis rate of global species
  double m_glob_clad_mainland;
};

///Anagenesis rate of all species
double calc_anagenesis(
  const double anagenesis_rate,
  const int n_global_species
);

///Anagenesis rate of all species
double calc_anagenesis(
  const parameters& p,
  const simulation& s
);

///Cladogesis rate of species on mainland
double calc_clad_mainland(
  const double clado_rate_main,
  const int n_mainland,
  const int n_mainland_only,
  const int carrying_cap_main
);

///Cladogesis rate of species on mainland
double calc_clad_mainland(
  const parameters& p,
  const simulation& s
);

///For a global species (that exist on both island and mainland),
/// the cladogenesis rate on the island
double calc_glob_clad_island(
  const double clado_rate_is,
  const int n_species_within_clade_d,
  const int carrying_cap_is,
  const int n_both
);

///For a global species (that exist on both island and mainland),
/// the cladogenesis rate on the island
double calc_glob_clad_island(
  const parameters& p,
  const simulation& s
);

///For a global species (that exist on both island and mainland),
/// the cladogenesis rate on the mainland
double calc_glob_clad_mainland(
  const parameters& p,
  const simulation& s
);

///For a global species (that exist on both island and mainland),
/// the rate at which it goes extinct on the mainland
double calc_glob_spec_ext_rate_on_mainland(
  const parameters& p,
  const simulation& s
);

///For a global species (that exist on both island and mainland),
/// the rate at which it goes extinct on the mainland
double calc_glob_spec_ext_rate_on_island(
  const parameters& p,
  const simulation& s
);

///For an island-only species
/// the rate at which it goes extinct on the island
double calc_islands_ext_rate_on_island(
  const parameters& p,
  const simulation& s
);

///For a mainland-only species
/// the rate at which it goes extinct on the mainland
double calc_mainlands_ext_rate_on_mainland(
  const parameters& p,
  const simulation& s
);

///Migration rate from mainland to to island
double calc_migration_to_island(
  const parameters& p,
  const simulation& s
);

//Calculates the rates
//mo number of mainland-only species
event_rates calculate_rates(
  const parameters& p,
  const simulation& s
);

//ratesvector: all rates stored in a vector
std::vector<double> to_ratesvector(const event_rates& r) noexcept;

//sumrates: sum of all rates
double calc_sumrates(const event_rates& r) noexcept;

} //~namespace elly

#endif // ELLY_RATES_H
