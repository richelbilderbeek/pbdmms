#ifndef ELLY_PARAMETERS_H
#define ELLY_PARAMETERS_H
#include <cmath>
#include <stdexcept>
#include <iosfwd>
#include <iostream>
#include <utility>

namespace elly {

class parameters
{
public:

  ///@param init_n_mainland initial number of species on the mainland.
  ///  For now, these are all in the same clade
  ///@param crown_age the crown age of the tree. Or: the time the simulation will take
  parameters(
    const double rate_clado_is,
    const double rate_clado_main,
    const double rate_ana,
    const double rate_ext_is,
    const double rate_ext_main,
    const double rate_mig_to_is,
    const int carryingcap_is,
    const int carryingcap_main,
    const int rng_seed,
    const int init_n_mainland,
    const double crown_age
  );

  ///rate of migration from mainland to island
  double get_mig_rate_to_island() const noexcept {return m_rate_mig_to_is; }

  ///rate of extinction island
  double get_ext_rate_is() const noexcept { return m_rate_ext_is; }

  ///rate of extinction mainland
  double get_ext_rate_main() const noexcept { return m_rate_ext_main; }

  ///rate of cladogenesis on island per species
  double get_clado_rate_is() const noexcept { return m_rate_clado_is; }

  ///rate of cladogenesis on mainland
  double get_clado_rate_main() const noexcept { return m_rate_clado_main; }

  ///rate of anagenesis, equal for mainland and island
  double get_ana_rate() const noexcept { return m_rate_ana; }

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
  const double m_rate_clado_is;

  ///rate of cladogenesis on mainland
  const double m_rate_clado_main;

  ///rate of anagenesis, irrelevant if on mainland or island
  const double m_rate_ana;

  ///rate of extinction island
  const double m_rate_ext_is;

  ///rate of extinction mainland
  const double m_rate_ext_main;

  ///rate of migration from mainland to island
  ///Other wat around is assumed to be zero
  const double m_rate_mig_to_is;

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

};


///rates extinction and cladogenesis same for mainland and island for now
parameters create_parameters_set1() noexcept;

parameters create_profiling_parameters() noexcept;

bool operator==(const parameters& lhs, const parameters& rhs) noexcept;

}//~namespace elly
#endif // ELLY_PARAMETERS_H
