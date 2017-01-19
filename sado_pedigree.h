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
  bool empty() const noexcept;
private:
  std::vector<indiv> m_v;
};

bool operator==(const pedigree& lhs, const pedigree& rhs) noexcept;

} //~namespace sado

#endif // SADO_PEDIGREE_H
