#include "elly_populations.h"

#include <algorithm>
#include <cassert>
#include <random>

#include "elly_location.h"
#include "elly_parameters.h"

elly::populations::populations(const parameters& p)
  : m_species{}
{
  const int n{p.get_init_n_mainland()};
  for (int i=0; i!=n; ++i)
  {
    m_species.push_back(create_new_test_species(location::mainland));
  }

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
  return std::count_if(
    std::begin(m_species),
    std::end(m_species),
    [](const auto& s)
    {
      return is_extinct(s);
    }
  );
}

int elly::populations::count_species(const location where) const noexcept
{
  return std::count_if(
    std::begin(m_species),
    std::end(m_species),
    [where](const auto& s)
    {
      return is_on(s, where);
    }
  );
}

int elly::populations::count_species(const clade_id& /* id */) const noexcept
{
  return count_species(location::both); //?Is this correct
}

std::vector<elly::species> elly::create_initial_mainland_species(const parameters& p)
{
  std::vector<species> v;
  const int n{p.get_init_n_mainland()};
  assert(n >= 0);
  v.reserve(n);

  for (int i=0; i!=n; ++i)
  {
    const species_id parent_id{create_null_species_id()};
    const species_id this_species_id{create_new_species_id()};
    //Initially, all clades start with one species
    const clade_id this_clade_id{create_new_clade_id()};
    const double time_of_birth{0.0};
    const location location_of_birth{location::mainland};

    const species s(
      this_species_id,
      parent_id,
      this_clade_id,
      time_of_birth,
      location_of_birth
    );
    v.push_back(s);
  }
  return v;
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

elly::species elly::populations::extract_random_species(
  const location where,
  std::mt19937& rng
  )
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
    throw std::logic_error("Cannot extract absent species");
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
  const species s = m_species[i];
  m_species[i] = m_species.back();
  m_species.pop_back();
  return s;
}

void elly::cladogenesis_mainland_only(
  populations& p,
  const double time,
  std::mt19937& rng
)
{
  species focal_species = p.extract_random_species(location::mainland_only, rng);

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

void elly::mainland_extinction(
  populations& p,
  const double time,
  std::mt19937& rng)
{
  species focal_species = p.extract_random_species(location::mainland_only, rng);
  
  //Make focal species go extinct
  focal_species.go_extinct(time, location::mainland);
  p.add_species(focal_species);
  
}

void elly::mainland_immigration(populations& p,
                               const double time,
                               std::mt19937& rng)
{
  species focal_species = p.extract_random_species(location::mainland, rng);
  //const int old_species_in_clade{p.count_species(focal_species.get_clade())};

  focal_species.migrate_to_island(time);
  p.add_species(focal_species);

  //const int new_species_in_clade{p.count_species(focal_species.get_clade())};
  //assert(new_species_in_clade == old_species_in_clade + 1);
}

void elly::island_extinction(populations& p, const double time, std::mt19937& rng)
{
  species focal_species = p.extract_random_species(location::island_only, rng);
  //const int old_species_in_clade{p.count_species(focal_species.get_clade())};

  focal_species.go_extinct(time, location::island);
  p.add_species(focal_species);

  //const int new_species_in_clade{p.count_species(focal_species.get_clade())};
  //assert(new_species_in_clade == old_species_in_clade - 1);
}

void elly::cladogenesis_island_only(populations& p, const double time, std::mt19937& rng)
{
  species focal_species = p.extract_random_species(location::island_only, rng);

  //const int old_species_in_clade{p.count_species(focal_species.get_clade())};

  focal_species.go_extinct(time, location::island);
  p.add_species(focal_species);

  //const int new_species_in_clade{p.count_species(focal_species.get_clade())};
  //assert(new_species_in_clade == old_species_in_clade + 1);

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

void elly::both_extinction_island(populations& p, const double time , std::mt19937& rng)
{
  species focal_species = p.extract_random_species(location::both, rng);
  //const int old_species_in_clade{p.count_species(focal_species.get_clade())};
  focal_species.go_extinct(time, location::island);
  p.add_species(focal_species);

  //const int new_species_in_clade{p.count_species(focal_species.get_clade())};
  //assert(new_species_in_clade == old_species_in_clade - 1);
}

void elly::both_extinction_mainland(populations& p, const double time , std::mt19937& rng)
{
  species focal_species = p.extract_random_species(location::both, rng);
  focal_species.go_extinct(time, location::mainland);
  p.add_species(focal_species);
}

void elly::both_anagenesis(populations& p, const double time, std::mt19937& rng)
{
  species focal_species = p.extract_random_species(location::both, rng);
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

void elly::cladogenesis_global_on_island(populations& p, const double time, std::mt19937& rng)
{
  species focal_species = p.extract_random_species(location::both, rng);

  //const int old_species_in_clade{p.count_species(focal_species.get_clade())};
  focal_species.go_extinct(time, location::island);
  p.add_species(focal_species);

  //const int new_species_in_clade{p.count_species(focal_species.get_clade())};
  //assert(new_species_in_clade == old_species_in_clade + 1);

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

void elly::cladogenesis_global_on_mainland(populations& p, const double time, std::mt19937& rng)
{
  species focal_species = p.extract_random_species(location::both, rng);
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
