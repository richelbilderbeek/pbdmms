#ifndef ELLY_PARAMETERS_H
#define ELLY_PARAMETERS_H

#include <iosfwd>
#include "elly_n_species.h"
#include "elly_carrying_capacity.h"
#include "elly_per_species_rate.h"
#include "elly_per_species_rates.h"

namespace elly {

class parameters
{
public:

  ///@param init_n_main_sps initial number of species on the mainland.
  ///@param init_n_main_clades initial number of clades on the mainland
  ///@param crown_age the crown age of the tree. Or: the time the simulation will take
  parameters(
    const per_species_rates& rates,
    const carrying_capacity carryingcap_is,
    const carrying_capacity carryingcap_main,
    const int rng_seed,
    const int init_n_main_clades,
    const int init_n_main_sps,
    const double crown_age
  );
  parameters(const parameters&) = default;
  parameters& operator=(const parameters&) = default;

  ///rate of migration from mainland to island
  per_species_rate get_mig_to_island() const noexcept {return m_rates.get_mig_to_is(); }

  ///rate of extinction island
  per_species_rate get_ext_is() const noexcept { return m_rates.get_ext_is(); }

  ///rate of extinction mainland
  per_species_rate get_ext_main() const noexcept { return m_rates.get_ext_main(); }

  ///rate of cladogenesis on island per species
  per_species_rate get_clado_is() const noexcept { return m_rates.get_clado_is(); }

  ///rate of cladogenesis on mainland
  per_species_rate get_clado_main() const noexcept { return m_rates.get_clado_main(); }

  ///rate of anagenesis, equal for mainland and island
  per_species_rate get_ana_rate() const noexcept { return m_rates.get_ana(); }

  ///carrying capacity per clade on island
  carrying_capacity get_carryingcap_is() const noexcept { return m_carryingcap_is; }

  ///carrying capacity per clade on mainland
  carrying_capacity get_carryingcap_main() const noexcept { return m_carryingcap_main; }

  ///per-species rates
  const per_species_rates& get_rates() const noexcept { return m_rates; }

  ///random number generator seed
  int get_rng_seed() const noexcept { return m_rng_seed; }

  ///Initial number of clades on the mainland
  int get_init_n_main_cls() const noexcept { return m_init_n_main_cls; }

  ///Initial number of species on the mainland
  int get_init_n_main_sps() const noexcept { return m_init_n_main_sps; }

  double get_crown_age() const noexcept { return m_crown_age; }

private:


  ///carrying capacity per clade on island
  carrying_capacity m_carryingcap_is;

  ///carrying capacity of all species on mainland
  carrying_capacity m_carryingcap_main;

  ///the crown age of the tree. Or: the time the simulation will take
  double m_crown_age;

  ///Initial number of clades on the mainland
  int m_init_n_main_cls;

  ///Initial number of species on the mainland
  int m_init_n_main_sps;

  ///The per-species rates
  per_species_rates m_rates;

  ///random number generator seed
  int m_rng_seed;

  friend std::ostream& operator<<(std::ostream& os, const parameters& p) noexcept;
  friend std::istream& operator>>(std::istream& os, parameters& p);
};

///rates extinction and cladogenesis same for mainland and island for now
parameters create_parameters_set1() noexcept;

///Parameter setting in which many things happen
parameters create_parameters_set2() noexcept;

///Parameter setting in which multiple colonizations by the same species
///takes place
parameters create_parameters_set3() noexcept;

parameters create_profiling_parameters() noexcept;

///Create a heading as if parameters would be one line
std::string get_parameters_heading() noexcept;

parameters load_parameters_from_file(const std::string& filename);

///Save parameters to file
void save(const parameters& p, const std::string& filename);

bool operator==(const parameters& lhs, const parameters& rhs) noexcept;
bool operator!=(const parameters& lhs, const parameters& rhs) noexcept;
std::ostream& operator<<(std::ostream& os, const parameters& p) noexcept;
std::istream& operator>>(std::istream& os, parameters& p);

carrying_capacity read_carrying_cap_is(std::istream& is);
carrying_capacity read_carrying_cap_main(std::istream& is);
double read_crown_age(std::istream& is);
int read_init_n_main_cls(std::istream& is);
int read_init_n_main_sps(std::istream& is);
per_species_rates read_rates(std::istream& is);
int read_rng_seed(std::istream& is);

}//~namespace elly
#endif // ELLY_PARAMETERS_H
