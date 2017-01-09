#ifndef ELLY_PARAMETERS_H
#define ELLY_PARAMETERS_H

#include <iosfwd>

#include "elly_per_species_rate.h"

namespace elly {

class parameters
{
public:

  ///@param init_n_mainland initial number of species on the mainland.
  ///  For now, these are all in the same clade
  ///@param crown_age the crown age of the tree. Or: the time the simulation will take
  parameters(
    const per_species_rate rate_clado_is,
    const per_species_rate rate_clado_main,
    const per_species_rate rate_ana,
    const per_species_rate rate_ext_is,
    const per_species_rate rate_ext_main,
    const per_species_rate rate_mig_to_is,
    const int carryingcap_is,
    const int carryingcap_main,
    const int rng_seed,
    const int init_n_mainland,
    const double crown_age
  );

  ///rate of migration from mainland to island
  per_species_rate get_mig_rate_to_island() const noexcept {return m_rate_mig_to_is; }

  ///rate of extinction island
  per_species_rate get_ext_rate_is() const noexcept { return m_rate_ext_is; }

  ///rate of extinction mainland
  per_species_rate get_ext_rate_main() const noexcept { return m_rate_ext_main; }

  ///rate of cladogenesis on island per species
  per_species_rate get_clado_rate_is() const noexcept { return m_rate_clado_is; }

  ///rate of cladogenesis on mainland
  per_species_rate get_clado_rate_main() const noexcept { return m_rate_clado_main; }

  ///rate of anagenesis, equal for mainland and island
  per_species_rate get_ana_rate() const noexcept { return m_rate_ana; }

  ///carrying capacity per clade on island
  int get_carryingcap_is() const noexcept { return m_carryingcap_is; }

  ///carrying capacity per clade on mainland
  int get_carryingcap_main() const noexcept { return m_carryingcap_main; }

  ///random number generator seed
  int get_rng_seed() const noexcept { return m_rng_seed; }

  ///Initial number of species on the mainland
  int get_init_n_mainland() const noexcept { return m_init_n_mainland; }

  auto get_crown_age() const noexcept { return m_crown_age; }

private:

  ///rate of cladogenesis on island per species
  const per_species_rate m_rate_clado_is;

  ///rate of cladogenesis on mainland
  const per_species_rate m_rate_clado_main;

  ///rate of anagenesis, irrelevant if on mainland or island
  const per_species_rate m_rate_ana;

  ///rate of extinction island
  const per_species_rate m_rate_ext_is;

  ///rate of extinction mainland
  const per_species_rate m_rate_ext_main;

  ///rate of migration from mainland to island
  ///Other wat around is assumed to be zero
  const per_species_rate m_rate_mig_to_is;

  ///carrying capacity per clade on island
  const int m_carryingcap_is;

  ///carrying capacity of all species on mainland
  const int m_carryingcap_main;

  ///random number generator seed
  const int m_rng_seed;

  ///Initial number of species on the mainland
  const int m_init_n_mainland;

  ///the crown age of the tree. Or: the time the simulation will take
  const double m_crown_age;

  friend std::ostream& operator<<(std::ostream& os, const parameters& p) noexcept;
};

///rates extinction and cladogenesis same for mainland and island for now
parameters create_parameters_set1(const int init_n_mainland = 20) noexcept;

///Parameter setting in which many things happen
parameters create_parameters_set2() noexcept;

parameters create_profiling_parameters() noexcept;

bool operator==(const parameters& lhs, const parameters& rhs) noexcept;
std::ostream& operator<<(std::ostream& os, const parameters& p) noexcept;

}//~namespace elly
#endif // ELLY_PARAMETERS_H
