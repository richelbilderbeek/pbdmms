#include "daic_species_status.h"
#include <string>
#include <stdexcept>

std::vector<daic::species_status> daic::collect_all_species_statuses() noexcept
{
  return
  {
    species_status::endemic_non_endemic,
    species_status::non_endemic,
    species_status::endemic,
    species_status::non_endemic_max_age
  };
}

std::string daic::to_str(const species_status s)
{
  if(s == species_status::endemic)
    return "Endemic";
  if(s == species_status::non_endemic)
    return "Non_endemic";
  if(s == species_status::endemic_non_endemic)
    return "Endemic&Non_Endemic";
  if(s == species_status::non_endemic_max_age)
    return "Non_endemic_MaxAge";
  throw std::invalid_argument("could not convert species_status to string");
}

daic::species_status daic::to_species_status(const std::string& s)
{
  if(s == "Endemic")
    return species_status::endemic;
  if(s == "Non_endemic")
    return species_status::non_endemic;
  if(s == "Endemic&Non_Endemic")
    return species_status::endemic_non_endemic;
  if(s == "Non_endemic_MaxAge")
    return species_status::non_endemic_max_age;
  throw std::invalid_argument("could not convert string to species_status");
}

std::ostream& daic::operator<<(std::ostream& os, const species_status s) noexcept
{
  os << to_str(s);
  return os;
}
