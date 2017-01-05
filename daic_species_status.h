#ifndef DAIC_SPECIES_STATUS_H
#define DAIC_SPECIES_STATUS_H

#include <string>

namespace daic {

/// +---------------------+--------------------+
/// | Enum value          | DAISIE             |
/// +---------------------+--------------------+
/// | non_endemic_max_age | Non_endemic_MaxAge |
/// | non_endemic         | Non_endemic        |
/// | endemic             | Endemic            |
/// +---------------------+--------------------+
enum class species_status
{
  non_endemic_max_age,
  non_endemic,
  endemic
};

///Converts the enum as is is used by DAISIE
/// +---------------------+--------------------+
/// | Enum value          | DAISIE             |
/// +---------------------+--------------------+
/// | non_endemic_max_age | Non_endemic_MaxAge |
/// | non_endemic         | Non_endemic        |
/// | endemic             | Endemic            |
/// +---------------------+--------------------+
std::string to_str(const species_status s);

species_status to_species_status(const std::string& s);

///Writes to stream in DAISIE form
std::ostream& operator<<(std::ostream& os, const input_row& r) noexcept;

} //~namespace daic

#endif // DAIC_SPECIES_STATUS_H
