#ifndef SADO_PEDIGREE_H
#define SADO_PEDIGREE_H

#include <vector>
#include "sado_individual.h"

namespace sado {

class pedigree
{
public:
  pedigree();
  void add(const indiv& i);
  bool are_related(const indiv& a, const indiv& b) const ;
  bool empty() const noexcept;

private:
  std::vector<indiv> m_v;

  friend bool operator==(const pedigree& lhs, const pedigree& rhs) noexcept;
};

bool operator==(const pedigree& lhs, const pedigree& rhs) noexcept;
bool operator!=(const pedigree& lhs, const pedigree& rhs) noexcept;

} //~namespace sado

#endif // SADO_PEDIGREE_H
