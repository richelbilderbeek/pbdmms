#ifndef ELLY_SPECIES_ID_H
#define ELLY_SPECIES_ID_H

#include <iosfwd>

namespace elly {

///Upon creation, created a unique ID
class species_id
{
public:
  int get_id() const noexcept { return m_id; }

private:

  ///Private constructor: the specied_id must be created by create_new_species_id,
  ///to explicitly show construction of it
  species_id(const int id);
  friend species_id create_new_species_id() noexcept;
  friend species_id create_null_species_id() noexcept;

  ///This ID
  int m_id;
};

bool operator==(const species_id lhs, const species_id rhs) noexcept;
bool operator!=(const species_id lhs, const species_id rhs) noexcept;
bool operator<(const species_id lhs, const species_id rhs) noexcept;

std::ostream& operator<<(std::ostream& os, const species_id id) noexcept;

species_id create_new_species_id() noexcept;

species_id create_null_species_id() noexcept;

} //~namespace elly

#endif // ELLY_SPECIES_ID_H
