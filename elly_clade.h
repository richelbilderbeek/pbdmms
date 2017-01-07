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
  explicit clade(
    const std::vector<species>& clade_species
  );
  const auto& get_species() const noexcept { return m_clade_species; }
  clade_id get_id() const noexcept;

  ///The current species is to be replaced with replacement
  /// * current must be present in the clade
  /// * replacement must have the same clade ID, species ID and parent ID
  void replace(const species& current, const species& replacement);

private:
  const std::vector<species> m_clade_species;
};

bool all_have_same_clade_id(const std::vector<species>& s);

///Collect the colonists in the clade
std::vector<species> collect_colonists(const clade& c) noexcept;

int count_colonists(const clade& c) noexcept;

int count_mainlanders(const clade& c) noexcept;

///Find the ancestral species of a species
species get_ancestor(const species s, const clade& c);

///Only keep those members that
/// * have colonized the island
/// * are born on the island
clade get_islanders(const clade& c);

///Get the species that has a certain species ID
species get_species_with_id(const species_id id, const clade& c);

///Instead of knowing the colonization times, we
///overestimate the colonization time to the common
///ancestor on the mainland
clade overestimate_colonization_time(clade c);

} //namespace elly

#endif // ELLY_CLADE_H
