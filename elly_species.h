#ifndef ELLY_SPECIES_H
#define ELLY_SPECIES_H

#include <iostream>
#include <stdexcept>
#include <vector>
#include "elly_species_id.h"
#include "elly_clade_id.h"
#include "elly_location.h"

namespace elly {

class species
{
public:
  ///@param location_of_birth location at which it is born. If the species is born on the
  ///   mainland, this can be both location::mainland and location::mainland_only.
  ///   If the species is born on the island, this can be both
  ///   location::island and location::island_only.
  species(
    const species_id this_species_id,
    const species_id parent_id,
    const clade_id this_clade_id,
    const double time_of_birth,
    const location location_of_birth
  );

  auto get_clade_id() const noexcept { return m_clade_id; }
  auto get_location_of_birth() const noexcept {  return m_location_of_birth; }
  auto get_parent_id() const noexcept {  return m_parent_id;  }
  auto get_species_id() const noexcept {  return m_species_id; }
  auto get_time_of_birth() const noexcept {  return m_time_of_birth; }
  auto get_time_of_colonization() const noexcept { return m_time_of_colonization; }
  auto get_time_of_extinction_mainland() const noexcept { return m_time_of_extinction_main; }
  auto get_time_of_extinction_island() const noexcept { return m_time_of_extinction_is; }

  void go_extinct(
    const double time_of_extinction,
    const location location_of_extinction,
    species& s
  );

  void migrate_to_island(const double colonization_time,
                         species& s);

  void set_time_of_colonisation(const double time_of_colonization, species& s);
  void set_time_of_extinction(const double time_of_extinction, const location place,
                              species& s);

private:

  ///clade index, const after construction
  clade_id m_clade_id;

  ///location of birth of this species
  ///const after construction
  location m_location_of_birth;

  ///parent species identifier, unique for each species
  ///const after construction
  species_id m_parent_id;

  ///species identifier, unique for each species
  ///const after construction
  species_id m_species_id;

  ///time of birth of this species
  ///const after construction
  double m_time_of_birth;

  ///time of colonization of the island,
  ///when species has not colonized an island, this is set equal to -1
  double m_time_of_colonization;

  ///time of extinction of species on mainland,
  /// when species is not extinct on island it is equal to -1
  double m_time_of_extinction_main;

  ///time of extinction of species on island,
  /// when species is not extinct on island it is equal to -1
  double m_time_of_extinction_is;

};

///Get the time of birth on the mainland. Will return -1.0 if the species
///is born on the island
double get_t_birth_mainland(const species& s) noexcept;

///Get the time of birth on the island. Will return -1.0 if the species
///is born on the mainland
double get_t_birth_island(const species& s) noexcept;

///Get the time the species went extinct on the mainland.
///Will return -1.0 if the species is extant on the mainland and not gone extinct yet
///Will return -1.0 if the species is absent on the mainland
double get_t_ext_mainland(const species& s) noexcept;

///Get the time the species went extinct on the island.
///Will return -1.0 if the species is extant on the island and not gone extinct yet
///Will return -1.0 if the species is absent on the island
double get_t_ext_island(const species& s) noexcept;

///Get the time the species colonized the island (from the mainland)
///Note that this simulation does not take into account that the mainland
///gets colonized by the island
double get_t_colonization(const species& s) noexcept;

///Is the species alive on either the island or mainland (or both)?
bool is_extant(const species& s) noexcept;

///Is the species extinct on both island and mainland?
bool is_extinct(const species& s) noexcept;

///Is the species extant on the island? This also includes a species
///that is present on both island and mainland
bool is_on_island(const species& s) noexcept;

///Is the species extant on the island only? This excludes a species
///that is present on both island and mainland
bool is_on_island_only(const species& s) noexcept;

///Is the species extant on the mainland? This also includes a species
///that is present on both island and mainland
bool is_on_mainland(const species& s) noexcept;

///Is the species extant on the mainland only? This excludes a species
///that is present on both island and mainland
bool is_on_mainland_only(const species& s) noexcept;

bool operator==(const species& lhs, const species& rhs) noexcept;
bool operator!=(const species& lhs, const species& rhs) noexcept;
bool operator<(const species& lhs, const species& rhs) noexcept;

///Create a new test species
species create_new_test_species(
  const double time_of_birth,
  const location location_of_birth
);

species create_new_test_species(
  const location location_of_birth
);

} //~namespace elly

#endif // ELLY_SPECIES_H
