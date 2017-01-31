#include "elly_location.h"

#include <cassert>
#include <stdexcept>

std::vector<elly::location> elly::collect_all_locations() noexcept
{
  return
  {
    location::mainland,
    location::mainland_only,
    location::island,
    location::island_only,
    location::both
  };
}

std::string elly::to_str(const location any_location) noexcept
{
  switch (any_location)
  {
    case location::mainland: return "mainland";
    case location::mainland_only: return "mainland_only";
    case location::island: return "island";
    case location::island_only: return "island_only";
    case location::both: return "both";
  }
  assert(!"Should not get here"); //!OCLINT accepted idiom, see Meyers' Effective C++
  throw std::logic_error("Unimplemented location");
}

std::ostream& elly::operator<<(std::ostream& os, const location any_location) noexcept
{
  os << to_str(any_location);
  return os;
}
