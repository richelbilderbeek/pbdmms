#ifndef ELLY_N_SPECIES_H
#define ELLY_N_SPECIES_H

#include <iostream>

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

double to_double(const n_species& n) noexcept;

bool operator==(const n_species lhs, const n_species rhs) noexcept;
bool operator!=(const n_species lhs, const n_species rhs) noexcept;
bool operator<(const n_species lhs, const n_species rhs) noexcept;
bool operator>(const n_species lhs, const n_species rhs) noexcept;
bool operator<=(const n_species lhs, const n_species rhs) noexcept;
bool operator>=(const n_species lhs, const n_species rhs) noexcept;
std::ostream& operator<<(std::ostream& os, const n_species& n) noexcept;
std::istream& operator>>(std::istream& is, n_species& n);

} //~namespace elly


#endif // ELLY_N_SPECIES_H
