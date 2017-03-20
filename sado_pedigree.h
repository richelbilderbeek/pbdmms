#ifndef SADO_PEDIGREE_H
#define SADO_PEDIGREE_H

#ifdef REALLY_NEED_PEDIGREE

#include <vector>
#include "sado_individual.h"
//#include "sado_indiv_graph.h"

namespace sado {

class pedigree
{
public:
  pedigree();
  void add(const indiv& i);
  bool are_related(const indiv& a, const indiv& b);
  bool check_parents_for_id(const indiv& a, const id& idnum);
  indiv get_indiv_from_id(const id& idnum);
  bool empty() const noexcept;

private:
  std::vector<indiv> m_v;
  friend bool operator==(const pedigree& lhs, const pedigree& rhs) noexcept;
};

bool operator==(const pedigree& lhs, const pedigree& rhs) noexcept;
bool operator!=(const pedigree& lhs, const pedigree& rhs) noexcept;

} //~namespace sado

#endif // REALLY_NEED_PEDIGREE

#endif // SADO_PEDIGREE_H
