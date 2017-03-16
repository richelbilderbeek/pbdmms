#include "sado_species.h"
#include <algorithm>
#include <cassert>
#include <iostream>

sado::species::species(
    const int gen,
    const std::vector<indiv>& indivs)
  : m_id{create_new_species_id()},
    m_generation{gen},
    m_indivs{std::begin(indivs), std::end(indivs)}
{
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
  return std::find_if(
    std::begin(m_indivs),
    std::end(m_indivs),
    [any_id](const auto& i)
    {
      return i.get_id() == any_id;
    }
  ) != std::end(m_indivs);
}

void sado::transfer_individuals(species& from, species& to)
{
  assert(from.get_generation() != -1);
  assert(from.get_generation() == to.get_generation());
  std::copy(
    std::begin(from.m_indivs),
    std::end(from.m_indivs),
    std::inserter(to.m_indivs, std::end(to.m_indivs))
  );
  //std::copy(
  //  std::begin(from.m_indivs),
  //  std::end(from.m_indivs),
  //  std::back_inserter(to.m_indivs)
  //);

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


