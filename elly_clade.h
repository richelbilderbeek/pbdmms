#ifndef ELLY_CLADE_H
#define ELLY_CLADE_H

#include <vector>
#include "elly_clade_id.h"
#include "elly_species.h"

namespace elly {

///A clade is a collection of species with the same clade ID
///It can be empty. When empty, 'get_id' will throw
class clade
{
public:
  explicit clade(
    const std::vector<species>& clade_species
  );
  const auto& get_species() const noexcept { return m_clade_species; }
  clade_id get_id() const;

  ///The current species is to be replaced with replacement
  /// * current must be present in the clade
  /// * replacement must have the same clade ID, species ID and parent ID
  void replace(const species& current, species replacement);

private:
  std::vector<species> m_clade_species;

  friend std::ostream& operator<<(std::ostream& os, const clade& c) noexcept;
};

bool all_have_same_clade_id(const std::vector<species>& s);

///Check if current can be replaced by replacement species
///Will throw if not
void check_can_replace(const species& current, species replacement);

///Collect the colonists in the clade
std::vector<species> collect_colonists(const clade& c) noexcept;

int conclude_n_missing_species(const clade& c);

int count_colonists(const clade& c) noexcept;

int count_mainlanders(const clade& c) noexcept;

///Find the ancestral species of a species
species get_ancestor(const species s, const clade& c);

///Only keep those members that
/// * have colonized the island
/// * are born on the island
///This function does not return a clade, as it may also return zero
///species. A clade has at least one species
std::vector<species> get_islanders(const std::vector<species>& v);

///Get the species that has a certain species ID
species get_species_with_id(
  const species_id id,
  const std::vector<species>& v
);

///Does species s have an ancestor in its clade?
bool has_ancestor(const species s, const clade& c) noexcept;

///See if a species with a certain species ID is present in the collection
bool has_species_with_id(
  const species_id id,
  const std::vector<species>& v
);

///If there are no species in this clade
bool is_empty(const clade& c) noexcept;

///If all species in this clade are extinct
///Will throw if the clade is empty
bool is_extinct(const clade& c);

///Collects kids from parent species
std::vector<species> collect_kids(const species& parent, const clade& c);

///Instead of knowing the colonization times, we
///overestimate the colonization time to the common
///ancestor on the mainland
///This function will return the clade as-is, if it
///is found in reality as such.
clade to_reality(clade c);

///Colonists have already been deduced
clade to_reality(clade c, const std::vector<species>& colonists);

///Colonist has already been deduced
clade to_reality(clade c, const species& colonist);


std::ostream& operator<<(std::ostream& os, const clade& c) noexcept;

} //namespace elly

#endif // ELLY_CLADE_H
