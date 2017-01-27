#include "elly_results.h"

#include <algorithm>
#include <iterator>
#include <iostream>
#include "daic_input_row.h"
#include "elly_clade.h"
#include "elly_clades.h"
#include "elly_results.h"
#include "elly_result.h"
#include "elly_simulation.h"
#include "elly_helper.h"
#include "elly_species.h"
#include <fstream>
#include <cassert>
#include <set>


elly::results::results(const std::vector<result>& r)
  : m_results{r}
{

}

bool elly::all_have_zero_kids(const std::vector<species>& v)
{
  return std::count_if(
    std::begin(v),
    std::end(v),
    [v](const species& x)
    {
      return collect_kids(x, v).size() == 0;
    }
  ) == static_cast<int>(v.size());
}

std::map<elly::clade_id, std::vector<elly::species>> elly::collect_clades_as_map(const results& r)
{
  std::map<clade_id, std::vector<species>> m;
  for (const result& p: r.get())
  {
    const auto& s = p.get_species();
    const auto iter = m.find(s.get_clade_id());
    if (iter == std::end(m))
    {
      std::vector<species> this_species = { s };
      m.insert(
        std::make_pair(
          s.get_clade_id(),
          this_species
        )
      );
    }
    else
    {
      (*iter).second.push_back(s);
    }
  }
  return m;
}

std::vector<elly::clade> elly::collect_clades_as_vector(const results& r)
{
  const std::map<clade_id, std::vector<species>> m = collect_clades_as_map(r);
  std::vector<clade> v;
  v.reserve(m.size());
  std::transform(
    std::begin(m),
    std::end(m),
    std::back_inserter(v),
    [](const auto& p) { return clade(p.second); }
  );
  return v;
}

elly::results elly::get_results(const simulation& s)
{
  return get_results(s.get_populations());
}

elly::results elly::get_results(const populations& p)
{
  std::vector<species> s{p.get_species()};
  std::sort(std::begin(s), std::end(s));

  std::vector<result> r;
  r.reserve(s.size());
  std::transform(
    std::begin(s),
    std::end(s),
    std::back_inserter(r),
    [](const species& i) { return result(i); }
  );
  return r;
}


std::vector<elly::species> elly::collect_colonists(const std::vector<species>& s)
{
  std::vector<species> ancestors;
  std::copy_if(
    std::begin(s),
    std::end(s),
    std::back_inserter(ancestors),
    [s](const species& candidate)
    {
      return
        candidate.get_location_of_birth() == location::mainland
        && !candidate.get_times_of_colonization().empty()
      ;
    }
  );
  return ancestors;
}


elly::species elly::find_youngest_colonist(const std::vector<species>& s)
{
  //return get_first_colonist(s);

  assert(s.size() > 1);
  assert(
    std::count_if(
      std::begin(s),
      std::end(s),
      [](const species& t)
      {
        return !t.get_times_of_colonization().empty();
      }
    ) >= 1
  );

  const std::vector<species> ancestors = collect_colonists(s);
  return get_first_colonist(ancestors);
}

std::vector<elly::species> elly::collect_kids(
  const species& parent,
  const std::vector<species>& population
)
{
  //The parent IDS that are in the family
  std::set<species_id> ids = { parent.get_species_id() };

  const int sz{static_cast<int>(population.size())};

  //We need to check the population sz times,
  //because the worst-case scenario is that all kids are decendants of each other
  for (int i{0}; i!=sz; ++i)
  {
    for (const auto& kid: population)
    {
      //If the kid has its parent ID in the IDs, it is a descendant
      if (ids.count(kid.get_parent_id()))
      {
        //It's ID is used to find its own descendants
        ids.insert(kid.get_species_id());
      }
    }
  }

  std::vector<species> kids; //descendants, excludes the parent itself
  std::copy_if(
    std::begin(population),
    std::end(population),
    std::back_inserter(kids),
    [ids](const auto& kid)
    {
      return ids.count(kid.get_parent_id());
    }
  );

  return kids;
}

//assumes vector of species that have lived on island and are in same clade
std::vector<double> elly::collect_branching_times(const clade& c)
{
  const auto& s = c.get_species();
  //Non-endemic species: only immigrated to island, but has not had anaganesis nor cladogenensis
  if (s.size() == 1)
  {
    const species& t = s[0];
    assert(t.get_location_of_birth() == location::mainland);
    assert(!t.get_times_of_colonization().empty());
    // #182: what to do with multiple colonizations?
    return
    {
      t.get_times_of_colonization().back()
    };
  }
  assert(!c.get_species().empty());

  const species parent = find_youngest_colonist(s);
  const std::vector<species> kids = collect_kids(parent, s);
  assert(!kids.empty());
  assert(!parent.get_times_of_colonization().empty());

  std::vector<double> branching_times = collect_speciation_times(kids);

  branching_times.push_back(
    get_last_colonization_of_youngest_colonist_before_speciation(c)
    //parent.get_times_of_colonization().front()
  );
  branching_times = get_with_duplicates_and_zeroes_removed(get_sorted(branching_times));
  assert(std::count(std::begin(branching_times), std::end(branching_times), 0.0) == 0);
  branching_times = get_sorted(branching_times);

  return branching_times;
}

bool elly::multiple_times_colonisation(const std::vector<species>& colonists)
{
  for(species x : colonists)
    {
      if(static_cast<int>(x.get_times_of_colonization().size()) > 1)
        {
        return true;
        }
    }
  return false;
}

daic::input elly::convert_clades_to_input(const clades& cs)
{
  std::vector<daic::input_row> rows;
  rows.reserve(cs.size());
  std::transform(
    std::begin(cs),
    std::end(cs),
    std::back_inserter(rows),
    [](const clade& c)
    {
      const std::string clade_name{std::to_string(c.get_id().get_id())};
      const auto status = conclude_status(c);
      const int n_missing_species{0};
      const auto branching_times = collect_branching_times(c);

      return daic::input_row(
         clade_name,
          status,
          n_missing_species,
          branching_times
      );
    }
  );
  return daic::input(rows);
}

daic::species_status elly::conclude_status(const clade& c)
{
  const auto& s = c.get_species();
  const std::vector<species> colonists = collect_colonists(s);
  if (all_have_zero_kids(s))
  {
    return daic::species_status::non_endemic;
  }

  if(colonists.size() > 1 || multiple_times_colonisation(colonists))
    {
      return daic::species_status::endemic_non_endemic;
    }
  if(colonists.size() == 1 && !multiple_times_colonisation(s))
    {
    return daic::species_status::endemic;
    }
  throw std::logic_error ("species_status could not be concluded");
}

daic::input_row elly::collect_info_clade(const clade& s)
{
  const daic::species_status status = conclude_status(s);
  const std::string clade_name{std::to_string(s.get_id().get_id())};
  const int n_missing_species{0};
  const std::vector<double> branching_times = collect_branching_times(s);
  return daic::input_row(
    clade_name,
    status,
    n_missing_species,
    branching_times
  );

}

daic::input elly::convert_ideal(const results& r)
{
  const clades clades_full = collect_clades_as_vector(r);
  assert(count_empty(clades_full) == 0);
  assert(clades_full.size() > 0);

  const clades islanders_with_empty = get_islanders(clades_full);
  assert(count_empty(islanders_with_empty) >= 0);
  assert(islanders_with_empty.size() > 0);

  //Will be empty if no colonizations occurred
  const clades cs{get_non_empty_clades(islanders_with_empty)};
  return convert_clades_to_input(cs);
}

daic::input elly::convert_reality(const results& r)
{
  //count clades on island
  const clades clades_full = collect_clades_as_vector(r);
  assert(count_empty(clades_full) == 0);
  assert(clades_full.size() > 0);

  //The species that need to be modified are:
  // * their mainland relatives have gone extinct
  //Time of colonization needs to be overestimated
  const clades colonization_known{
    to_reality(clades_full)
  };
  assert(count_empty(colonization_known) == 0);

  const clades islanders_with_empty = get_islanders(colonization_known);
  assert(count_empty(islanders_with_empty) >= 0);
  assert(islanders_with_empty.size() > 0);

  //Will be empty if no colonizations occurred
  const clades cs{get_non_empty_clades(islanders_with_empty)} ;
  return convert_clades_to_input(cs);
}

bool elly::is_empty(const results& r) noexcept
{
  return r.get().empty();
}

std::ostream& elly::operator<<(std::ostream& os, const results& r) noexcept
{
  os << get_result_header() << '\n';
  std::copy(
    std::begin(r.m_results),
    std::end(r.m_results),
    std::ostream_iterator<result>(os, "\n")
  );
  return os;
}
