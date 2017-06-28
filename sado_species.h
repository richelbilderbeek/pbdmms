#ifndef SADO_SPECIES_H
#define SADO_SPECIES_H

#include "sado_individual.h"
#include "phyg_species_id.h"
#include "phyg_species.h"
#include <iosfwd>
#include <vector>
#include <set>

namespace sado {

using species = phyg::species<sado::individual>;

/*
class species
{
public:
  explicit species(
    const int gen = -1,
    const std::vector<individual>& indivs = {});

  bool empty() const noexcept { return m_indivs.empty(); }
  auto get_id() const noexcept { return m_id; }
  const auto& get_indivs() const noexcept {return m_indivs;}
  auto get_generation() const noexcept { return m_generation;}
  const auto size() const noexcept { return m_indivs.size(); }

  ///Is this individual present? Assumes that all individuals have a unique ID
  bool has_individual(const individual& i) const noexcept { return has_individual(i.get_id()); }
  ///Is there an individual with this (individual) ID present?
  // Must be fast to make has_ancestor_and_kid fast
  bool has_individual(const id any_id) const noexcept;

private:
  phyg::species_id m_id;
  int m_generation;
  std::vector<individual> m_indivs;

  ///For fast-look only, duplicates the IDs of m_indivs
  std::set<id> m_individual_ids;

  friend void transfer_individuals(species& from, species& to);

};

///Collect the IDs of all individuals
std::vector<id> collect_individual_ids(const std::vector<individual>& indivs);

///Find if the 'ancestors' contain at least one father or mother
///of one of the individuals in 'kids'
bool has_ancestor_and_kid(const species& ancestors, const species& kids);

///Transfers the individuals from 'from' to 'to'
///Assumes the species are from the same generation
void transfer_individuals(species& from, species& to);

bool operator==(const species &lhs, const species &rhs) noexcept;
bool operator!=(const species &lhs, const species &rhs) noexcept;

std::ostream& operator<<(std::ostream& os, const species& spp) noexcept;
*/

}//~namespace sado

#endif // SADO_SPECIES_H
