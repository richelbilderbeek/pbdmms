#include "elly_results.h"

#include <algorithm>
#include <iterator>
#include <iostream>
#include "daic_input_row.h"
#include "elly_clade.h"
#include "elly_results.h"
#include "elly_result.h"
#include "elly_simulation.h"
#include "elly_species.h"
#include <fstream>
#include <cassert>
#include <set>

elly::results::results(const std::vector<result>& r)
  : m_results{r}
{

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
    [](const auto& p) { return p.second; }
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


std::vector<elly::species> elly::collect_ancestors(const std::vector<species>& s)
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
        && candidate.get_time_of_colonization() >= 0.0
        && collect_kids(candidate, s).size() > 0
      ;
    }
  );
  return ancestors;
}

elly::species elly::find_youngest_parent(std::vector<species> s)
{
  assert(s.size() > 1);
  assert(
    std::count_if(
      std::begin(s),
      std::end(s),
      [](const species& t)
      {
        return t.get_time_of_colonization() != -1.0;
      }
    ) >= 1
  );

  const std::vector<species> ancestors = collect_ancestors(s);
  //Find oldest ancestor, time of colonization is smalles
  return *std::min_element(
    std::begin(ancestors),
    std::end(ancestors),
    [](const species& lhs, const species& rhs)
    {
      return lhs.get_time_of_colonization() < rhs.get_time_of_colonization();
    }
  );

  /*
  double colonization_time{-2.0};
  species parent(create_new_test_species(location::mainland));
  assert(parent.get_location_of_birth() == location::mainland);
  for(species x: s)
  {
  if(x.get_time_of_colonization() != -1.0)
    {
    if(x.get_time_of_colonization() < colonization_time)
      {
        assert(x.get_time_of_colonization() != 0.0);
      colonization_time = x.get_time_of_colonization();
      parent = x;
      }
    }
  }
  assert(colonization_time != -2.0);
  assert(colonization_time != -1.0);
  assert(parent.get_location_of_birth() != location::both);
  return parent;
  */
}
std::vector<elly::species> elly::collect_kids(
  const species& parent,
  const std::vector<species>& population
)
{
  //The parent IDS that are in the family
  std::set<species_id> ids = { parent.get_species_id() };

  const int sz = population.size();

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
    assert(t.get_time_of_colonization() != -1.0);
    return { t.get_time_of_colonization() };
  }
  const species parent = find_youngest_parent(s);
  const std::vector<species> kids = collect_kids(parent, s);
  assert(!kids.empty());
  std::vector<double> branching_times;
  for(species x: kids)
  {
    if(x.get_time_of_colonization() == -1.0)
      branching_times.push_back(x.get_time_of_birth());
  }
  std::sort(branching_times.begin(), branching_times.end());
  auto last = std::unique(branching_times.begin(), branching_times.end());
  branching_times.erase(last, branching_times.end());

  return branching_times;
}

//Ask Rampal about this later
daic::species_status elly::conclude_status(const clade& c)
{
  const auto& s = c.get_species();
  if(s.size() == 1 && s[0].get_location_of_birth() == location::mainland)
    {
    return daic::species_status::non_endemic;
    }
  return daic::species_status::endemic;
}

/*
///ELLY_TODO
void save_daisie_results_with_main_ext(const elly::results &r)
{
  std::ofstream ofs(filename);
  //todo define status per clade
  for(const elly::species& p: r)
    {
      if(p.get_t_colonization() != -1.0)
        {

        }
    }
}

void save_daisie_results_without_main_ext();

*/

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

daic::input elly::convert_to_daisie_input_with_main_ext(const results& r)
{
  //count clades on island
  const std::vector<clade> clades = collect_clades_as_vector(r);

  //The species that need to be modified are:
  // * are non-endemic
  // * their mainland relatives have gone extinct
  //Time of colonization needs to be overestimated
  std::cerr << __func__ << ": TODO\n";

  std::vector<daic::input_row> rows;
  rows.reserve(clades.size());
  std::transform(
    std::begin(clades),
    std::end(clades),
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

daic::input elly::convert_to_daisie_input_without_main_ext(const results& r)
{
  const std::vector<clade> clades = collect_clades_as_vector(r);

  std::vector<daic::input_row> rows;
  rows.reserve(clades.size());
  std::transform(
    std::begin(clades),
    std::end(clades),
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
