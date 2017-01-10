#ifndef ELLY_PER_SPECIES_RATES_H
#define ELLY_PER_SPECIES_RATES_H

#include <iosfwd>
#include "elly_per_species_rate.h"

namespace elly {

///The per_species_rates used in the parameters
class per_species_rates
{
public:
  per_species_rates(
    const per_species_rate ana = per_species_rate(0.0),
    const per_species_rate clado_is = per_species_rate(0.0),
    const per_species_rate clado_main = per_species_rate(0.0),
    const per_species_rate ext_is = per_species_rate(0.0),
    const per_species_rate ext_main = per_species_rate(0.0),
    const per_species_rate mig_to_is = per_species_rate(0.0)
  );

  auto get_ana() const noexcept { return m_ana; }
  auto get_clado_is() const noexcept { return m_clado_is; }
  auto get_clado_main() const noexcept { return m_clado_main; }
  auto get_ext_is() const noexcept { return m_ext_is; }
  auto get_ext_main() const noexcept { return m_ext_main; }
  auto get_mig_to_is() const noexcept { return m_mig_to_is; }

private:

  ///rate of anagenesis, irrelevant if on mainland or island
  per_species_rate m_ana;

  ///rate of cladogenesis on island per species
  per_species_rate m_clado_is;

  ///rate of cladogenesis on mainland
  per_species_rate m_clado_main;

  ///rate of extinction island
  per_species_rate m_ext_is;

  ///rate of extinction mainland
  per_species_rate m_ext_main;

  ///migration rate
  per_species_rate m_mig_to_is;

  friend std::ostream& operator<<(std::ostream& os, const per_species_rates& r) noexcept;
  friend std::istream& operator>>(std::istream& is, per_species_rates& r);
  friend bool operator==(const per_species_rates& lhs, const per_species_rates& rhs) noexcept;
};

per_species_rates create_test_rates_1() noexcept;
per_species_rates create_test_rates_2() noexcept;

std::ostream& operator<<(std::ostream& os, const per_species_rates& r) noexcept;
std::istream& operator>>(std::istream& is, per_species_rates& r);

bool operator==(const per_species_rates& lhs, const per_species_rates& rhs) noexcept;
bool operator!=(const per_species_rates& lhs, const per_species_rates& rhs) noexcept;

} //~namespace elly

#endif // ELLY_PER_SPECIES_RATES_H
