#include "sado_erasure_method.h"

#include <stdexcept>

std::vector<sado::erasure_method> sado::collect_all_erasure_methods() noexcept
{
  return
  {
    erasure_method::erase,
    erasure_method::swap
  };
}

sado::erasure_method sado::to_erasure_method(const std::string &s)
{
  if (s == "erase")
    return erasure_method::erase;
  if (s == "swap")
    return erasure_method::swap;
  throw std::invalid_argument("cannot convert string to erasure");
}

std::string sado::to_str(const erasure_method m) noexcept
{
  if (m == erasure_method::erase)
    return "erase";
  if (m == erasure_method::swap)
    return "swap";
  throw std::invalid_argument("cannot convert erasure to string");
}

std::ostream& sado::operator<<(std::ostream& os, const erasure_method m) noexcept
{
  os << to_str(m);
  return os;
}
