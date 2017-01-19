#include "sado_pedigree.h"

sado::pedigree::pedigree()
{

}

bool sado::pedigree::empty() const noexcept
{
  return true;
}

bool sado::operator==(const pedigree& /* lhs */, const pedigree& /* rhs */) noexcept
{
  return true;
}
