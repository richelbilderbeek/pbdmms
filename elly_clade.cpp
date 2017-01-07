#include "elly_clade.h"

#include <algorithm>
#include <cassert>
#include <vector>
#include <iterator>

#include "elly_species.h"

elly::clade::clade(
  const std::vector<species>& clade_species
) : m_clade_species{clade_species}
{
  if (!m_clade_species.empty()
    && !all_have_same_clade_id(m_clade_species))
  {
    throw std::invalid_argument("Species must all have the same clade ID");
  }
}

bool elly::all_have_same_clade_id(const std::vector<species>& s)
{
  if (s.empty())
  {
    throw std::invalid_argument(
      "Cannot determine clade IDs are all the same for an empty collection");
  }
  const species first = s[0];
  return std::all_of(s.begin(), s.end(),
    [first] (const species t)
    { return t.get_clade_id() == first.get_clade_id(); });
}

void elly::check_can_replace(const species& current, species replacement)
{
  if (current.get_species_id() != replacement.get_species_id())
  {
    throw std::invalid_argument("Replacement must be of same species");
  }
  if (current.get_parent_id() != replacement.get_parent_id())
  {
    throw std::invalid_argument("Replacement must have same parent");
  }
  if (current.get_location_of_birth() != replacement.get_location_of_birth())
  {
    throw std::invalid_argument("Replacement must have same birth location");
  }
  if (current.get_time_of_birth() != replacement.get_time_of_birth())
  {
    throw std::invalid_argument("Replacement must have same time of birth");
  }
}

std::vector<elly::species> elly::collect_colonists(const clade& c) noexcept
{
  const auto& s = c.get_species();
  std::vector<species> colonists;
  std::copy_if(
    std::begin(s),
    std::end(s),
    std::back_inserter(colonists),
    [](const auto& t)
    {
      return is_colonist(t);
    }
  );
  return colonists;
}

int elly::count_colonists(const clade& c) noexcept
{
  const auto& s = c.get_species();
  return std::count_if(
    std::begin(s),
    std::end(s),
    [](const auto& t)
    {
      return is_colonist(t);
    }
  );
}

int elly::count_mainlanders(const clade& c) noexcept
{
  const auto& s = c.get_species();
  return std::count_if(
    std::begin(s),
    std::end(s),
    [](const auto& t)
    {
      return is_mainlander(t);
    }
  );
}

elly::species elly::get_ancestor(
  const species s,
  const clade& c
)
{
  const auto id = s.get_parent_id();
  return get_species_with_id(id, c.get_species());
}

elly::clade_id elly::clade::get_id() const
{
  if (m_clade_species.empty())
  {
    throw std::logic_error("Cannot get am empty clade its ID");
  }
  return m_clade_species.back().get_clade_id();
}

std::vector<elly::species> elly::get_islanders(const std::vector<species>& v)
{
  std::vector<species> w;
  std::copy_if(
    std::begin(v),
    std::end(v),
    std::back_inserter(w),
    [](const species& s)
    {
      return is_islander(s);
    }
  );
  return w;
}

elly::species elly::get_species_with_id(
  const species_id id,
  const std::vector<species>& v)
{
  const auto iter = find_if(
    std::begin(v),
    std::end(v),
    [id](const species& s)
    {
      return s.get_species_id() == id;
    }
  );
  if (iter == std::end(v))
  {
    throw std::invalid_argument("Species' ID absent in clade");
  }
  return *iter;
}

void elly::clade::replace(const species& current, species replacement)
{
  if (m_clade_species.empty())
  {
    throw std::invalid_argument("Cannot replace a species in an empty clade");
  }
  if (get_id() != current.get_clade_id())
  {
    throw std::invalid_argument(
      "Cannot replace a species that is absent in this clade");
  }
  if (get_id() != replacement.get_clade_id())
  {
    throw std::invalid_argument("Clade ID of replacement different from clade");
  }
  check_can_replace(current, replacement);

  const auto iter = std::find(
    std::begin(m_clade_species),
    std::end(m_clade_species),
    current);
  if (iter == std::end(m_clade_species))
  {
    throw std::invalid_argument("Cannot replace absent species");
  }
  std::swap(*iter, replacement);
}

elly::clade elly::overestimate_colonization_time(clade c)
{
  if (!count_colonists(c) || !count_mainlanders(c))
  {
    throw std::logic_error(
      "Cannot overestimate colonization time if there is no "
      "colonist or mainland ancestor");
  }

  assert(count_colonists(c) == 1);
  assert(count_mainlanders(c) >= 1);
  const std::vector<species> colonists = collect_colonists(c);
  assert(colonists.size() == 1);
  const species colonist = colonists.back();
  const species ancestor = get_ancestor(colonist, c);
  assert(ancestor.get_time_of_birth() <= colonist.get_time_of_birth());
  assert(ancestor.get_time_of_extinction_mainland() == colonist.get_time_of_birth());
  assert(ancestor.get_location_of_birth() == location::mainland);
  assert(ancestor.get_time_of_extinction_mainland() >= 0.0);
  species overestimated_colonist = colonist;
  const double t_colonization_new {
    ancestor.get_time_of_extinction_mainland()
  };
  overestimated_colonist.set_time_of_colonisation(t_colonization_new);
  c.replace(colonist, overestimated_colonist);
  return c;
}
