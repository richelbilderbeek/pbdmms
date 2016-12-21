#ifndef ELLY_SPECIES_H
#define ELLY_SPECIES_H

#include <iostream>
#include <stdexcept>
#include <vector>
#include "elly_species_id.h"
#include "elly_clade_id.h"

namespace elly {

class species
{
public:
  species(
    const double time_of_birth,
    const species_id parent_id,
    const species_id this_species_id,
    const clade_id this_clade_id
  );

  auto get_species_id() const noexcept {  return m_species_id; }
  auto get_parent_id() const noexcept {  return m_parent_id;  }
  auto get_time_of_birth() const noexcept {  return m_time_of_birth; }
  auto get_time_of_extinction() const noexcept {  return m_time_of_extinction; }
  auto get_clade() const noexcept { return m_clade; }

  ///When a species goes from mainland to island. If a
  void set_time_of_colonisation(const double time_of_extinction);
  void set_time_of_extinction(const double time_of_extinction);

private:
  ///time of birth of this species
  ///const after construction
  double m_time_of_birth;

  ///parent species identifier, unique for each species
  ///const after construction
  species_id m_parent_id;

  ///species identifier, unique for each species
  ///const after construction
  species_id m_species_id;

  ///time of extinction, when species is not extinct it is equal to -1
  ///Unknown at time of construction :-)
  double m_time_of_extinction;

  ///clade index, const after construction
  clade_id m_clade;
};

bool is_extant(const species& s) noexcept;

bool is_extinct(const species& s) noexcept;

} //~namespace elly

#endif // ELLY_SPECIES_H
