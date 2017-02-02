#include "sado_pedigree.h"

sado::pedigree::pedigree()
{

}

void sado::pedigree::add(const indiv& i)
{
  m_v.push_back(i);
}

bool sado::pedigree::are_related(const indiv& /* a */, const indiv& /* b */) const
{
  return false;
}

bool sado::pedigree::empty() const noexcept
{
  return m_v.empty();
}

bool sado::operator==(const pedigree& lhs, const pedigree& rhs) noexcept
{
  return lhs.m_v == rhs.m_v;
}

bool sado::operator!=(const pedigree& lhs, const pedigree& rhs) noexcept
{
  return !(lhs == rhs);
}
