#include "kewe_output_parameters.h"

#include <iostream>

std::ostream& kewe::operator<<(std::ostream& os, const output_parameters p) noexcept
{
  os
    << "STUB"
    << p.histbinp
  ;
  return os;
}
