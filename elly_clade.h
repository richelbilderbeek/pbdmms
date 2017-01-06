#ifndef ELLY_CLADE_H
#define ELLY_CLADE_H

#include <vector>
#include "elly_clade_id.h"
#include "elly_species.h"


namespace elly {

///A clade is a collection of species with the same clade ID
///Must be at least one species
class clade
{
public:
  clade(
    const std::vector<species>& clade_species
  );
  const auto& get_species() const noexcept { return m_clade_species; }
  clade_id get_id() const noexcept;

  const std::vector<species> m_clade_species;
};

bool all_have_same_clade_id(const std::vector<species>& s);

///Only keep those members that
/// * have colonized the island
/// * are born on the island
clade get_islanders(const clade& c);

///Instead of knowing the colonization times, we
///overestimate the colonization time to the common
///ancestor on the mainland
clade overestimate_colonization_time(const clade& c);

} //namespace elly

#endif // ELLY_CLADE_H
