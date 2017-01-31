#ifndef DAIC_SPECIES_STATUS_H
#define DAIC_SPECIES_STATUS_H

#include <string>
#include <vector>

namespace daic {

/// +---------------------+--------------------+
/// | Enum value          | DAISIE             |
/// +---------------------+--------------------+
/// | Endemic&Non_Endemic | Endemic&Non_Endemic|
/// | non_endemic         | Non_endemic        |
/// | endemic             | Endemic            |
/// | Non_endemic_MaxAge  | Non_endemic_MaxAge |
/// +---------------------+--------------------+
enum class species_status
{
  endemic_non_endemic,
  non_endemic,
  endemic,
  non_endemic_max_age
};

std::vector<species_status> collect_all_species_statuses() noexcept;

///Converts the enum as is is used by DAISIE
/// +---------------------+--------------------+
/// | Enum value          | DAISIE             |
/// +---------------------+--------------------+
/// | Endemic&Non_Endemic | Endemic&Non_Endemic|
/// | non_endemic         | Non_endemic        |
/// | endemic             | Endemic            |
/// | Non_endemic_MaxAge  | Non_endemic_MaxAge |
/// +---------------------+--------------------+
std::string to_str(const species_status s);

species_status to_species_status(const std::string& s);

///Writes to stream in DAISIE form
std::ostream& operator<<(std::ostream& os, const species_status s) noexcept;

} //~namespace daic

#endif // DAIC_SPECIES_STATUS_H
