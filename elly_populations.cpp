#include "elly_populations.h"

#include <algorithm>
#include <cassert>
#include <random>
#include <set>

#include "elly_clade.h"
#include "elly_location.h"
#include "elly_parameters.h"
#include "elly_helper.h"
#include "elly_clade_id.h"

elly::populations::populations(const parameters& p)
  : m_species{create_initial_mainland_species(p)}
{

}

elly::populations::populations(const std::vector<species>& species
) : m_species{species}
{

}

void elly::populations::add_species(const species& s)
{
  m_species.push_back(s);
}

int elly::populations::count_extinct_species() const noexcept
{
  return count_extinct(m_species);
}

int elly::count_extinct(const populations& p) noexcept
{
  return count_extinct(p.get_species());
}

int elly::count_extinct(const std::vector<species>& p) noexcept
{
  return std::count_if(
    std::begin(p),
    std::end(p),
    [](const auto& s)
    {
      return is_extinct(s);
    }
  );
}

int elly::count_is_on(const populations& p, const location any_location) noexcept
{
  return count_is_on(p.get_species(), any_location);
}

int elly::count_is_on(const std::vector<species>& p, const location any_location) noexcept
{
  return std::count_if(
    std::begin(p),
    std::end(p),
    [any_location](const species& s)
    {
      return is_on(s, any_location);
    }
  );
}

int elly::populations::count_species(const location where) const noexcept
{
  return count_is_on(m_species, where);
}

int elly::populations::count_species(const clade_id& /* id */) const noexcept
{
  return count_species(location::both); //?Is this correct
}

std::vector<elly::species> elly::create_initial_mainland_species(const parameters& p)
{
  std::vector<clade_id> clade_ids;
  const int n_clades{p.get_init_n_main_cls()};
  for (int i=0; i!=n_clades; ++i)
  {
    clade_ids.push_back(create_new_clade_id());
  }

  std::vector<species> pop;
  const int n_species{p.get_init_n_main_sps()};
  for (int i=0; i!=n_species; ++i)
  {
    const species s(
      create_new_species_id(),
      create_null_species_id(),
      clade_ids[i % n_clades],
      0.0,
      location::mainland
    );
    pop.push_back(s);
  }
  return pop;
}

elly::populations elly::create_test_populations_1()
{
  ///Create a population with
  /// * 1 island-only species
  /// * 1 mainland-only species
  /// * 1 global species
  /// * 1 extinct species
  /// The chronology is
  /// * at t=0.0, a was born on the mainland and is mainland-only
  /// * at t=1.0, b was born on the mainland and colonized the island at t=1.5,
  ///   thus is global, derived from a
  /// * at t=2.0, c was born on the island and is island only, derived from #2
  /// * at t=2.0, d was born on the island and is island only, went extinct at t=3.0
  const double t1{0.0};
  const double t2{1.0};
  const double t3{2.0};
  const double t4{2.0};

  const species a = create_new_test_species(t1, location::mainland);
  species b = create_new_test_species(t2, location::mainland);
  const species c = create_new_test_species(t3, location::island);
  species d = create_new_test_species(t4, location::island);
  b.migrate_to_island(1.5);
  d.go_extinct(3.0, location::island);
  return populations( {a, b, c, d} );
}

elly::species elly::populations::get_random_species(
  const location where,
  std::mt19937& rng
) const
{
  const int n_species{static_cast<int>(m_species.size())};
  std::vector<int> candidate_indicess;
  for (int i{0}; i!=n_species; ++i)
  {
    if(is_on(m_species[i], where))
    {
      candidate_indicess.push_back(i);
    }
  }
  if (candidate_indicess.empty())
  {
    throw std::logic_error("Cannot get a species that is absent");
  }
  const int n_candidates{static_cast<int>(candidate_indicess.size())};
  std::uniform_int_distribution<int> distr(0, n_candidates - 1);
  const int candidate_index = distr(rng);
  assert(candidate_index >= 0);
  assert(candidate_index < static_cast<int>(candidate_indicess.size()));

  //Extract focal species from mainland, remove it from that vector
  const int i = candidate_indicess[candidate_index];
  assert(i >= 0);
  assert(i < static_cast<int>(m_species.size()));
  return m_species[i];
}

elly::species elly::get_species_with_id(
  const species_id id,
  const populations& p
)
{
  return get_species_with_id(id, p.get_species());
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

int elly::count_clades(const populations& pop)
{
  std::vector<species> s = pop.get_species();
  std::set<clade_id> c;
  assert(!s.empty());
  for(species i: s)
    {
      c.insert(i.get_clade_id());
    }
  assert(!c.empty());
  return c.size();
}

void elly::cladogenesis_mainland_only(
  populations& p,
  const double time,
  std::mt19937& rng
)
{
  const species focal_species = p.get_random_species(location::mainland_only, rng);
  cladogenesis_mainland_only(p, time, focal_species);
}

void elly::cladogenesis_mainland_only(
  populations& p,
  const double time,
  const species& s
)
{
  if(is_on_island_only(s) || is_on_both(s))
    {
      throw std::logic_error("event::clad_main_only can only happen with mainland only species");
    }
  species focal_species = p.extract_species(s);

  //Make that focal species go extict
  focal_species.go_extinct(time, location::mainland);
  p.add_species(focal_species);

  //Give birth to two new lineages
  const species derived_a(
    create_new_species_id(),
    focal_species.get_species_id(), //parent_id
    focal_species.get_clade_id(), //clade_id
    time, //time of birth,
    location::mainland

  );
  p.add_species(derived_a);

  const species derived_b(
    create_new_species_id(),
    focal_species.get_species_id(), //parent_id
    focal_species.get_clade_id(), //clade_id
    time, //time of birth
    location::mainland
  );
  p.add_species(derived_b);
}

elly::species elly::populations::extract_species(const species& s)
{
  const auto i = std::find(
    std::begin(m_species), std::end(m_species), s);
  assert(i != std::end(m_species));
  std::swap(*i, m_species.back());
  m_species.pop_back();
  return s;
}

elly::species elly::populations::extract_random_species(
  const location where,
  std::mt19937& rng
)
{
  const species s = get_random_species(where, rng);
  return extract_species(s);
}

void elly::mainland_extinction(
  populations& p,
  const double time,
  std::mt19937& rng)
{
  species focal_species = p.get_random_species(location::mainland_only, rng);
  mainland_extinction(p, time, focal_species);
}

void elly::mainland_extinction(
  populations& p,
  const double time,
  const species& s)
{
  if (is_on_island_only(s) || is_on_both(s))
  {
    throw std::logic_error("event::ext_main_only can only happen on mainland-only species");
  }
  species focal_species = p.extract_species(s);

  //Make focal species go extinct
  focal_species.go_extinct(time, location::mainland);
  p.add_species(focal_species);
}

void elly::mainland_immigration(
  populations& p,
  const double time,
  std::mt19937& rng)
{
  const species focal_species = p.get_random_species(location::mainland, rng);
  mainland_immigration(p, time, focal_species);
}

void elly::mainland_immigration(
  populations& p,
  const double time,
  const species& s)
{
  if(is_on_island_only(s))
    {
      throw std::logic_error("endemic species cannot migrate to island");
    }

  species focal_species = p.extract_species(s);

  focal_species.migrate_to_island(time);
  p.add_species(focal_species);
}

void elly::island_extinction(
  populations& p,
  const double time,
  std::mt19937& rng)
{
  const species focal_species = p.get_random_species(location::island_only, rng);
  island_extinction(p, time, focal_species);
}

void elly::island_extinction(
  populations& p,
  const double time,
  const species& s)
{
  if(is_on_mainland_only(s) || is_on_both(s))
    {
      throw std::logic_error("event::ext_island_only can only be done on island_only species");
    }

  species focal_species = p.extract_species(s);

  focal_species.go_extinct(time, location::island);
  p.add_species(focal_species);
}


void elly::cladogenesis_island_only(
  populations& p,
  const double time,
  std::mt19937& rng)
{
  assert(p.count_species(location::island_only) > 0);
  const species focal_species = p.get_random_species(location::island_only, rng);
  cladogenesis_island_only(p, time, focal_species);
}

void elly::cladogenesis_island_only(
  populations& p,
  const double time,
  const species& s)
{
  if(is_on_mainland_only(s) || is_on_both(s))
    {
      throw std::logic_error("event::clad_island_only can only be done on island_only species");
    }
  assert(p.count_species(location::island_only) > 0);
  species focal_species = p.extract_species(s);

  focal_species.go_extinct(time, location::island);
  p.add_species(focal_species);

  //Give birth to two new lineages
  const species derived_a(
    create_new_species_id(),
    focal_species.get_species_id(), //parent_id
    focal_species.get_clade_id(), //clade_id
    time, //time of birth
    location::island
  );
  p.add_species(derived_a);

  const species derived_b(
    create_new_species_id(),
    focal_species.get_species_id(), //parent_id
    focal_species.get_clade_id(), //clade_id
    time, //time of birth
    location::island
  );
  p.add_species(derived_b);
}

void elly::both_extinction_island(
  populations& p,
  const double time,
  std::mt19937& rng)
{
  const species focal_species = p.get_random_species(location::both, rng);
  both_extinction_island(p, time, focal_species);
}

void elly::both_extinction_island(
  populations& p,
  const double time,
  const species& s)
{
  if(is_on_island_only(s) || is_on_mainland_only(s))
    {
      throw std::logic_error("event::ext_glob_on_island can only happen with global species");
    }
  species focal_species = p.extract_species(s);
  focal_species.go_extinct(time, location::island);
  p.add_species(focal_species);
}

void elly::both_extinction_mainland(
  populations& p,
  const double time,
  std::mt19937& rng)
{
  const species focal_species = p.get_random_species(location::both, rng);
  both_extinction_mainland(p, time, focal_species);
}

void elly::both_extinction_mainland(
  populations& p,
  const double time,
  const species& s)
{
  if(is_on_island_only(s) || is_on_mainland_only(s))
    {
      throw std::logic_error("event::ext_glob_on_main can only happen with global species");
    }
  species focal_species = p.extract_species(s);
  focal_species.go_extinct(time, location::mainland);
  p.add_species(focal_species);
}

void elly::both_anagenesis(populations& p, const double time, std::mt19937& rng)
{
  const species s = p.get_random_species(location::both, rng);
  return both_anagenesis(p, time, s);
}

void elly::both_anagenesis(populations& p, const double time, const species& s)
{
  if(is_on_island_only(s) || is_on_mainland_only(s))
    {
      throw std::logic_error("event::ana can only happen with global species");
    }
  species focal_species = p.extract_species(s);
  focal_species.go_extinct(time, location::island);
  p.add_species(focal_species);

  const species derived(
    create_new_species_id(),
    focal_species.get_species_id(), //parent_id
    focal_species.get_clade_id(),
    time, //time of birth
    location::island
  );
  p.add_species(derived);
}

void elly::cladogenesis_global_on_island(
  populations& p,
  const double time,
  std::mt19937& rng)
{
  const species focal_species = p.get_random_species(location::both, rng);
  cladogenesis_global_on_island(p, time, focal_species);
}

void elly::cladogenesis_global_on_island(
  populations& p,
  const double time,
  const species& s)
{
  if(is_on_island_only(s) || is_on_mainland_only(s))
    {
      throw std::logic_error("event::clad_glob_on_island can only happen with global species");
    }
  species focal_species = p.extract_species(s);
  focal_species.go_extinct(time, location::island);
  p.add_species(focal_species);

  //Give birth to two new lineages
  const species derived_a(
    create_new_species_id(),
    focal_species.get_species_id(), //parent_id
    focal_species.get_clade_id(), //clade_id
    time, //time of birth
    location::island
  );
  p.add_species(derived_a);

  const species derived_b(
    create_new_species_id(),
    focal_species.get_species_id(), //parent_id
    focal_species.get_clade_id(), //clade_id
    time, //time of birth
    location::island
  );
  p.add_species(derived_b);
}

void elly::cladogenesis_global_on_mainland(
  populations& p,
  const double time,
  std::mt19937& rng)
{
  const species focal_species = p.get_random_species(location::both, rng);
  cladogenesis_global_on_mainland(p, time, focal_species);
}

void elly::cladogenesis_global_on_mainland(
  populations& p,
  const double time,
  const species& s)
{
  if(is_on_island_only(s) || is_on_mainland_only(s))
    {
      throw std::logic_error("event::clad_glob_on_main can only happen with global species");
    }
  species focal_species = p.extract_species(s);
  focal_species.go_extinct(time, location::mainland);
  p.add_species(focal_species);

  //Give birth to two new lineages
  const species derived_a(
    create_new_species_id(),
    focal_species.get_species_id(), //parent_id
    focal_species.get_clade_id(), //clade_id
    time, //time of birth
    location::mainland
  );
  p.add_species(derived_a);

  const species derived_b(
    create_new_species_id(),
    focal_species.get_species_id(), //parent_id
    focal_species.get_clade_id(), //clade_id
    time, //time of birth
    location::mainland
  );
  p.add_species(derived_b);
}
