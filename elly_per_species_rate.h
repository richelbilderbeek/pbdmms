#ifndef ELLY_PER_SPECIES_RATE_H
#define ELLY_PER_SPECIES_RATE_H

namespace elly {

///Rate that something happens per timestep, must be positive
class per_species_rate
{
public:
  per_species_rate(const double r);

  double get() const noexcept { return m_r; }

private:
  double m_r;
};

bool operator==(const per_species_rate& lhs, const per_species_rate& rhs) noexcept;

} //~namespace elly

#endif // ELLY_PER_SPECIES_RATE_H
