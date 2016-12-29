#include "sado_gausser_implementation.h"

#include <stdexcept>

sado::gausser_implementation sado::to_gausser_implementation(const std::string& s)
{
  if (s == "lut") return gausser_implementation::lut;
  if (s == "raw") return gausser_implementation::raw;
  throw std::invalid_argument("cannot convert string to gausser_implementation");
}
