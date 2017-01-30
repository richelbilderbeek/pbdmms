#include "sado_gausser_implementation.h"

#include <stdexcept>

sado::gausser_implementation
sado::to_gausser_implementation(const std::string &s)
{
  if (s == "lut")
    return gausser_implementation::lut;
  if (s == "raw")
    return gausser_implementation::raw;
  throw std::invalid_argument(
      "cannot convert string to gausser_implementation");
}

std::string sado::to_str(const gausser_implementation g) noexcept
{
  if (g == gausser_implementation::lut)
    return "lut";
  if (g == gausser_implementation::raw)
    return "raw";
  throw std::invalid_argument(
      "cannot convert gausser_implementation to string");
}

std::ostream &sado::
operator<<(std::ostream &os, const gausser_implementation g) noexcept
{
  os << to_str(g);
  return os;
}
