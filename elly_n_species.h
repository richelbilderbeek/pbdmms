#ifndef ELLY_N_SPECIES_H
#define ELLY_N_SPECIES_H

namespace elly {

///Number of species, must be zero or more
class n_species
{
public:
  n_species(const int n);

  int get() const noexcept { return m_n; }

private:
  int m_n;
};

bool operator==(const n_species lhs, const n_species rhs) noexcept;
bool operator<(const n_species lhs, const n_species rhs) noexcept;
bool operator<=(const n_species lhs, const n_species rhs) noexcept;

} //~namespace elly


#endif // ELLY_N_SPECIES_H
