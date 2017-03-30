#include "sado_next_generation_method.h"
#include <stdexcept>

std::vector<sado::next_generation_method> sado::collect_all_next_generation_methods() noexcept
{
  return
  {
    next_generation_method::overlapping,
    next_generation_method::seperate
  };
}

sado::next_generation_method sado::to_next_generation_method(const std::string& s)
{
  if (s == "overlapping")
    return next_generation_method::overlapping;
  if (s == "seperate")
    return next_generation_method::seperate;
  throw std::invalid_argument(
      "cannot convert string to next_generation_method");
}

std::string sado::to_str(const next_generation_method e) noexcept
{
  if (e == next_generation_method::overlapping)
    return "overlapping";
  if (e == next_generation_method::seperate)
    return "seperate";
  throw std::invalid_argument(
    "cannot convert next_generation_method to string");
}

std::ostream& sado::operator<<(std::ostream& os, const next_generation_method e) noexcept
{
  os << to_str(e);
  return os;
}
