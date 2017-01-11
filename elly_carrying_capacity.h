#ifndef ELLY_CARRYING_CAPACITY_H
#define ELLY_CARRYING_CAPACITY_H

#include <iosfwd>
#include "elly_n_species.h"

namespace elly {

///Number of species, must be zero or more
class carrying_capacity
{
public:
  carrying_capacity(const n_species n);

  n_species get() const noexcept { return m_n; }

private:
  n_species m_n;
};

double to_double(const carrying_capacity& k) noexcept;

bool operator==(const carrying_capacity lhs, const carrying_capacity rhs) noexcept;
bool operator!=(const carrying_capacity lhs, const carrying_capacity rhs) noexcept;
bool operator<(const carrying_capacity lhs, const carrying_capacity rhs) noexcept;
bool operator>(const carrying_capacity lhs, const carrying_capacity rhs) noexcept;
bool operator<=(const carrying_capacity lhs, const carrying_capacity rhs) noexcept;
bool operator>=(const carrying_capacity lhs, const carrying_capacity rhs) noexcept;
std::ostream& operator<<(std::ostream& os, const carrying_capacity& n) noexcept;
std::istream& operator>>(std::istream& is, carrying_capacity& n);

} //~namespace elly


#endif // ELLY_CARRYING_CAPACITY_H
