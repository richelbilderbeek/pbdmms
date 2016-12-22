#include "kewe_ploidy.h"

#include <cassert>
#include <iostream>

std::string kewe::to_string(const ploidy p) noexcept
{
  if (p == ploidy::diploid)
  {
    return "diploid";
  }
  else
  {
    assert(p == ploidy::haploid);
    return "haploid";
  }
}

std::ostream& kewe::operator<<(std::ostream& os, const ploidy p) noexcept
{
  os << to_string(p);
  return os;
}
