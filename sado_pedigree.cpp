#include "sado_pedigree.h"

sado::pedigree::pedigree()
{

}

void sado::pedigree::add(const indiv& i)
{
  m_v.push_back(i);
}

bool sado::pedigree::empty() const noexcept
{
  return true;
}

bool sado::operator==(const pedigree& /* lhs */, const pedigree& /* rhs */) noexcept
{
  return true;
}
