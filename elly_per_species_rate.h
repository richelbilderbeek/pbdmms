#ifndef ELLY_PER_SPECIES_RATE_H
#define ELLY_PER_SPECIES_RATE_H

#include <iosfwd>

namespace elly {

///Rate that something happens per timestep, must be positive
class per_species_rate
{
public:
  explicit per_species_rate(const double r = 0.0);

  double get() const noexcept { return m_r; }

private:
  double m_r;
};

bool operator==(const per_species_rate& lhs, const per_species_rate& rhs) noexcept;
bool operator!=(const per_species_rate& lhs, const per_species_rate& rhs) noexcept;
std::ostream& operator<<(std::ostream& os, const per_species_rate& r) noexcept;
std::istream& operator>>(std::istream& is, per_species_rate& r);

} //~namespace elly

#endif // ELLY_PER_SPECIES_RATE_H
