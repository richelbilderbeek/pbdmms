#ifndef SADO_PEDIGREE_H
#define SADO_PEDIGREE_H

namespace sado {

class pedigree
{
public:
  pedigree();
  bool empty() const noexcept;
private:
  //No idea yet
};

bool operator==(const pedigree& lhs, const pedigree& rhs) noexcept;

} //~namespace sado

#endif // SADO_PEDIGREE_H
