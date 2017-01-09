#include "elly_clade.h"

#include <algorithm>
#include <cassert>
#include <vector>
#include <iterator>
#include <sstream>
#include "elly_clade.h"
#include "elly_species.h"
#include "elly_results.h"

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

int elly::conclude_n_missing_species(const clade& c)
{
  int n_missing_species{0};
  std::vector<species> colonists = collect_colonists(c);
  const species ancestor = get_youngest_colonist(colonists);
  std::vector<double> time_diversification = get_time_of_birth_children(ancestor);
  //removing doubles
  std::sort(time_diversification.begin(), time_diversification.end());
  auto last = std::unique(time_diversification.begin(), time_diversification.end());
  time_diversification.erase(last, time_diversification.end());
  const auto new_end = std::remove(time_diversification.begin(), time_diversification.end(), 0.0);
  time_diversification.erase(new_end, std::end(time_diversification));

  if(time_diversification.size() == 1)
    {
      return n_missing_species;
    }
  else
    {

    }
  /*
  if(count_colonists(c) == 1)
    {
      return 0;
    }
  double t_colonisation{0.0};
  int relevant_species{0};
  int all_species{0};
  std::vector<elly::species> colonists = collect_colonists(c);
  for(species colonist : colonists)
    {
      all_species += static_cast<int>(collect_kids(colonist, c).size());
      if(colonist.get_time_of_colonization() > t_colonisation)
        {
          t_colonisation = colonist.get_time_of_colonization();
          relevant_species = static_cast<int>(collect_kids(colonist, c).size()) + 1;
        }
    }
  return all_species - relevant_species;
  */
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

bool elly::has_ancestor(const species s, const clade& c) noexcept
{
  const auto id = s.get_parent_id();
  return has_species_with_id(id, c.get_species());
}

bool elly::has_species_with_id(
  const species_id id,
  const std::vector<species>& v
)
{
  try
  {
    get_species_with_id(id, v);
    return true;
  }
  catch (std::invalid_argument&)
  {
    return false;
  }
}

bool elly::is_empty(const clade& c) noexcept
{
  return c.get_species().empty();
}

bool elly::is_extinct(const clade& c)
{
  const auto& v = c.get_species();
  const int n_extant{
    static_cast<int>(
      std::count_if(
        std::begin(v),
        std::end(v),
        [](const species& s)
        {
          return is_extant(s);
        }
      )
    )
  };
  return n_extant == 0;
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

std::vector<elly::species> elly::collect_kids(const species& parent, const clade& c)
{
  std::vector<species> population = c.get_species();
  return collect_kids(parent, population);
}

elly::clade elly::to_reality(clade c)
{
  if (is_extinct(c))
  {
    return c;
  }
  if (!count_colonists(c) || !count_mainlanders(c))
  {
    return c;
  }
  assert(count_colonists(c) >= 1);
  assert(count_mainlanders(c) >= 1);
  return to_reality(c, collect_colonists(c));
}

elly::clade elly::to_reality(clade c, const std::vector<species>& colonists)
{
  assert(count_colonists(c) >= 1);
  assert(count_mainlanders(c) >= 1);
  if (colonists.size() == 1)
  {
    return to_reality(c, colonists.back());
  }
  throw std::runtime_error("Multiple colonists not implemented yet");
}

elly::clade elly::to_reality(clade c, const species& colonist)
{
  assert(count_colonists(c) >= 1);
  assert(count_mainlanders(c) >= 1);
  //If there is a mainland conspecific, we can reliably estimate
  //the time of colonization
  if (is_on_both(colonist))
  {
    return c;
  }

  assert(!is_on_both(colonist));

  //If it has no ancestor, it has a sister species at time = 0.0
  if (!has_ancestor(colonist, c))
  {
    species overestimated_col = colonist;
    const double t_colonization_new {0.0};
    overestimated_col.set_time_of_colonisation(t_colonization_new);
    c.replace(colonist, overestimated_col);
    return c;
  }

  assert(has_ancestor(colonist, c));

  const species ancestor = get_ancestor(colonist, c);
  assert(ancestor.get_time_of_birth() <= colonist.get_time_of_birth());
  assert(ancestor.get_time_of_extinction_mainland() == colonist.get_time_of_birth());
  assert(ancestor.get_location_of_birth() == location::mainland);
  assert(ancestor.get_time_of_extinction_mainland() >= 0.0);
  species overestimated_col = colonist;
  const double t_colonization_new {
    ancestor.get_time_of_extinction_mainland()
  };
  overestimated_col.set_time_of_colonisation(t_colonization_new);
  c.replace(colonist, overestimated_col);
  return c;
}


std::ostream& elly::operator<<(std::ostream& os, const clade& c) noexcept
{
  std::stringstream s;
  const auto& v = c.get_species();
  s
    << "CID: " << c.get_id()
    << " with " << v.size() << " species" << '\n';
  for (const auto& this_species: v)
  {
    s << " * " << this_species << '\n';
  }
  std::string t{s.str()};
  assert(!t.empty());
  t.resize(t.size() - 1);
  os << t;
  return os;
}
