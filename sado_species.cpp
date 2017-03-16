#include "sado_species.h"
#include <algorithm>
#include <cassert>
#include <iostream>

sado::species::species(
    const int gen,
    const std::vector<indiv>& indivs)
  : m_id{create_new_species_id()},
    m_generation{gen},
    m_indivs{indivs},
    m_individual_ids{to_set(collect_individual_ids(indivs))}
{
  assert(all_have_unique_ids(m_indivs));
  assert(m_indivs.size() == m_individual_ids.size());
}

std::vector<sado::id> sado::collect_individual_ids(const std::vector<indiv>& indivs)
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

bool sado::has_ancestor_and_kid(const species& ancestors, const species& kids)
{
  for (const auto& kid: kids.get_indivs())
  {
    if (ancestors.has_individual(kid.get_father_id())
      || ancestors.has_individual(kid.get_mother_id())
    ) return true;
  }
  return false;
}

bool sado::species::has_individual(const id any_id) const noexcept
{
  // O(log(n)) lookup
  return m_individual_ids.find(any_id) != std::end(m_individual_ids);

  // O(n) lookup
  //  return std::find_if(
  //    std::begin(m_indivs),
  //    std::end(m_indivs),
  //    [any_id](const auto& i)
  //    {
  //      return i.get_id() == any_id;
  //    }
  //  ) != std::end(m_indivs);
}

std::set<sado::id> sado::to_set(const std::vector<id>& v)
{
  return std::set<id>(std::begin(v), std::end(v));
}

void sado::transfer_individuals(species& from, species& to)
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

bool sado::operator==(const species& lhs, const species& rhs) noexcept
{
  return lhs.get_indivs() == rhs.get_indivs() && lhs.get_id() == rhs.get_id();
}
bool sado::operator!=(const species& lhs, const species& rhs)  noexcept
{
  return !(lhs == rhs);
}

std::ostream& sado::operator<<(std::ostream& os, const species& spp) noexcept
{
  os << spp.get_id() << ":" << spp.get_generation(); //STUB
  return os;
}


