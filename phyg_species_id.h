#ifndef PHYG_SPECIES_ID_H
#define PHYG_SPECIES_ID_H

#include <iosfwd>

namespace phyg {

///Upon creation, creates a unique ID, to be used for species
///For individuals, use the 'id' class
class species_id
{
public:
  int get_id() const noexcept { return m_id; }

private:

  ///Private constructor: the specied_id must be created by create_new_id,
  ///to explicitly show construction of it
  species_id(const int species_id);
  friend species_id create_new_species_id() noexcept;
  friend species_id create_null_species_id() noexcept;
  friend species_id create_negative_of(const species_id id);

  ///This ID
  int m_id;
};

bool operator==(const species_id lhs, const species_id rhs) noexcept;
bool operator!=(const species_id lhs, const species_id rhs) noexcept;
bool operator<(const species_id lhs, const species_id rhs) noexcept;
bool operator>(const species_id lhs, const species_id rhs) noexcept;
std::ostream& operator<<(std::ostream& os, const species_id id) noexcept;

species_id create_new_species_id() noexcept;

species_id create_null_species_id() noexcept;

} //~namespace phyg

#endif // PHYG_SPECIES_ID_H
