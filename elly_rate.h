#ifndef ELLY_RATE_H
#define ELLY_RATE_H

#include "elly_per_species_rate.h"
#include "elly_n_species.h"

namespace elly {

///Rate at whcih something happens per timestep, must be zero or positive
class rate
{
public:
  explicit rate(const double r);
  rate(const rate&) = default;
  rate& operator=(const rate&) = default;

  double get() const noexcept { return m_r; }
private:
  double m_r;
};

bool operator==(const rate lhs, const rate rhs) noexcept;
bool operator!=(const rate lhs, const rate rhs) noexcept;
bool operator<(const rate lhs, const rate rhs) noexcept;
bool operator<=(const rate lhs, const rate rhs) noexcept;

///The rate of an event happening, per timestep
/// @param psr per-species-rate of the event happening, per timestep
/// @param n number of species
rate operator*(const per_species_rate psr, const n_species n) noexcept;
rate operator*(const n_species n, const per_species_rate psr) noexcept;


} //~namespace elly

#endif // ELLY_RATE_H
