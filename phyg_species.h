#ifndef PHYG_SPECIES_H
#define PHYG_SPECIES_H

#include "phyg_helper.h"
#include "phyg_id.h"
#include "phyg_species_id.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>
#include <set>

namespace phyg {

///Collect the IDs of all individuals
template <class individual>
std::vector<id> collect_individual_ids(const std::vector<individual>& indivs)
{
  std::vector<id> ids;
  ids.reserve(indivs.size());
  std::transform(
    std::begin(indivs),
    std::end(indivs),
    std::back_inserter(ids),
    [](const auto& i)
    {
      return i.get_id();
    }
  );
  return ids;
}

///Transfers the individuals from 'from' to 'to'
///Assumes the species are from the same generation
template <class species>
void transfer_individuals(species& from, species& to)
{
  assert(from.get_generation() != -1);
  assert(from.get_generation() == to.get_generation());
  std::copy(
    std::begin(from.m_indivs),
    std::end(from.m_indivs),
    std::back_inserter(to.m_indivs)
  );

  from.m_indivs.clear();
  from.m_generation = -1;
}

template <class individual>
class species
{
public:
  explicit species(
    const int gen = -1,
    const std::vector<individual>& indivs = {})
  : m_id{phyg::create_new_species_id()},
    m_generation{gen},
    m_indivs{indivs},
    m_individual_ids{to_set(collect_individual_ids(indivs))}
  {
    assert(all_have_unique_ids(m_indivs));
    assert(m_indivs.size() == m_individual_ids.size());
  }

  bool empty() const noexcept { return m_indivs.empty(); }
  auto get_id() const noexcept { return m_id; }
  const auto& get_indivs() const noexcept {return m_indivs;}
  auto get_generation() const noexcept { return m_generation;}
  const auto size() const noexcept { return m_indivs.size(); }

  ///Is this individual present? Assumes that all individuals have a unique ID
  bool has_individual(const individual& i) const noexcept { return has_individual(i.get_id()); }

  ///Is there an individual with this (individual) ID present?
  // Must be fast to make has_ancestor_and_kid fast
  bool has_individual(const id any_id) const noexcept
  {
    return m_individual_ids.find(any_id) != std::end(m_individual_ids);
  }

private:
  phyg::species_id m_id;
  int m_generation;
  std::vector<individual> m_indivs;

  ///For fast-look only, duplicates the IDs of m_indivs
  std::set<id> m_individual_ids;

  friend void transfer_individuals<species>(species& from, species& to);

};

///Find if the 'ancestors' contain at least one father or mother
///of one of the individuals in 'kids'
template <class individual>
bool has_ancestor_and_kid(
  const species<individual>& ancestors,
  const species<individual>& kids
)
{
  for (const auto& kid: kids.get_indivs())
  {
    if (ancestors.has_individual(kid.get_father_id())
      || ancestors.has_individual(kid.get_mother_id())
    ) return true;
  }
  return false;
}

template <class individual>
bool operator==(const species<individual> &lhs, const species<individual> &rhs) noexcept
{
  return lhs.get_indivs() == rhs.get_indivs() && lhs.get_id() == rhs.get_id();
}

template <class individual>
bool operator!=(const species<individual> &lhs, const species<individual> &rhs) noexcept
{
  return !(lhs == rhs);
}

template <class individual>
std::ostream& operator<<(std::ostream& os, const species<individual>& spp) noexcept
{

  os << "SID:" << spp.get_id() << ", "
    << "t:" << spp.get_generation() << ", "
    << "#individuals: " << spp.get_indivs().size()
  ;
  return os;
}

}//~namespace phyg

#endif // PHYG_SPECIES_H
