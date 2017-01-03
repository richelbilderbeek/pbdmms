#include "elly_populations.h"

#include <cassert>
#include <random>

#include "elly_location.h"
#include "elly_parameters.h"

elly::populations::populations(const parameters& p)
  : m_extinct_species{},
    m_species_both{},
    m_species_island{},
    m_species_mainland{create_initial_mainland_species(p)}
{

}

elly::populations::populations(
  std::vector<species> extinct_species,
  std::vector<species> species_both,
  std::vector<species> species_island,
  std::vector<species> species_mainland
) : m_extinct_species{extinct_species},
    m_species_both{species_both},
    m_species_island{species_island},
    m_species_mainland{species_mainland}
{

}

void elly::populations::add_extinct_species(const species& s)
{
  assert(is_extinct(s));
  this->m_extinct_species.push_back(s);
}

void elly::populations::add_species_mainland(const species& s)
{
  assert(is_extant(s));
  m_species_mainland.push_back(s);
}

void elly::populations::add_species_island(const species& s)
{
  assert(is_extant(s));
  m_species_island.push_back(s);
}

void elly::populations::add_species_both(const species& s)
{
  assert(is_extant(s));
  m_species_both.push_back(s);
}

int elly::populations::count_extinct_species() const noexcept
{
  return static_cast<int>(this->m_extinct_species.size());
}

int elly::populations::count_species(const location where) const noexcept
{
  switch (where)
  {
    case location::both: return static_cast<int>(this->m_species_both.size());
    case location::island_only: return static_cast<int>(this->m_species_island.size());
    case location::mainland_only: return static_cast<int>(this->m_species_mainland.size());
    case location::island:
      return count_species(location::island_only)
        + count_species(location::both)
      ;
    case location::mainland:
      return count_species(location::mainland_only)
        + count_species(location::both)
      ;
  }
  assert(!"Should not get here"); //!OCLINT accepted idiom
  throw std::logic_error("Should not get here");
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
  const species_id id1{create_new_species_id()};
  const species_id id2{create_new_species_id()};
  const species_id id3{create_new_species_id()};
  const species_id id4{create_new_species_id()};
  const species_id pid1{create_null_species_id()};
  const species_id pid2{id1};
  const species_id pid3{id2};
  const species_id pid4{id2};
  const clade_id cid1{create_new_clade_id()};
  const clade_id cid2{cid1};
  const clade_id cid3{cid1};
  const clade_id cid4{cid1};
  const species a(id1, pid1, cid1, t1, location::mainland);
  species b(id2, pid2, cid2, t2, location::mainland);
  const species c(id3, pid3, cid3, t3, location::island);
  species d(id4, pid4, cid4, t4, location::island);
  b.migrate_to_island(1.5);
  d.go_extinct(3.0, location::island);

  const std::vector<species> extinct_species = {d};
  const std::vector<species> species_both = {b};
  const std::vector<species> species_island = {c};
  const std::vector<species> species_mainland = {a};
  return populations(
    extinct_species,
    species_both,
    species_island,
    species_mainland
  );
}

*/
elly::species elly::populations::extract_random_species(
  std::vector<species>& v,
  std::mt19937& rng
  )
{
  assert(!v.empty());
  std::uniform_int_distribution<int> species_indices(0, v.size() - 1);
  const int n = species_indices(rng);

  assert(n >= 0);
  assert(n < static_cast<int>(v.size()));

  //Extract focal species from mainland, remove it from that vector
  const species s = v[n];
  v[n] = v.back();
  v.pop_back();
  return s;
}

elly::species elly::populations::extract_random_both_species(std::mt19937& rng)
{
  assert(!m_species_both.empty());
  return extract_random_species(m_species_both, rng);
}

elly::species elly::populations::extract_random_island_species(std::mt19937& rng)
{
  assert(!m_species_island.empty());
  return extract_random_species(m_species_island, rng);
}

elly::species elly::populations::extract_random_mainland_species(std::mt19937& rng)
{
  assert(!m_species_mainland.empty());
  return extract_random_species(m_species_mainland, rng);
}

void elly::mainland_cladogenesis(
  populations& p,
  const double time,
  std::mt19937& rng
)
{
  species focal_species = p.extract_random_mainland_species(rng);

  //Make that focal species go extict
  focal_species.go_extinct(time, location::mainland);
  p.add_extinct_species(focal_species);

  //Give birth to two new lineages
  const species derived_a(
    create_new_species_id(),
    focal_species.get_species_id(), //parent_id
    focal_species.get_clade_id(), //clade_id
    time, //time of birth,
    location::mainland

  );
  p.add_species_mainland(derived_a);

  const species derived_b(
    create_new_species_id(),
    focal_species.get_species_id(), //parent_id
    focal_species.get_clade_id(), //clade_id
    time, //time of birth
    location::mainland
  );
  p.add_species_mainland(derived_b);
}

void elly::mainland_extinction(
  populations& p,
  const double time,
  std::mt19937& rng)
{
  species focal_species = p.extract_random_mainland_species(rng);
  
  //Make focal species go extinct
  focal_species.go_extinct(time, location::mainland);
  p.add_extinct_species(focal_species);
  
}

void elly::mainland_immigration(populations& p,
                               const double time,
                               std::mt19937& rng)
{
  species focal_species = p.extract_random_mainland_species(rng);
  //const int old_species_in_clade{p.count_species(focal_species.get_clade())};

  focal_species.migrate_to_island(time);
  p.add_species_both(focal_species);

  //const int new_species_in_clade{p.count_species(focal_species.get_clade())};
  //assert(new_species_in_clade == old_species_in_clade + 1);
}

void elly::island_extinction(populations& p, const double time, std::mt19937& rng)
{
  species focal_species = p.extract_random_island_species(rng);
  //const int old_species_in_clade{p.count_species(focal_species.get_clade())};

  focal_species.go_extinct(time, location::island);
  p.add_extinct_species(focal_species);

  //const int new_species_in_clade{p.count_species(focal_species.get_clade())};
  //assert(new_species_in_clade == old_species_in_clade - 1);
}

void elly::island_cladogenesis(populations& p, const double time, std::mt19937& rng)
{
  species focal_species = p.extract_random_island_species(rng);

  //const int old_species_in_clade{p.count_species(focal_species.get_clade())};

  focal_species.go_extinct(time, location::island);
  p.add_extinct_species(focal_species);

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
  p.add_species_island(derived_a);

  const species derived_b(
    create_new_species_id(),
    focal_species.get_species_id(), //parent_id
    focal_species.get_clade_id(), //clade_id
    time, //time of birth
    location::island
  );
  p.add_species_island(derived_b); 
}

void elly::both_extinction_island(populations& p, const double /* time */, std::mt19937& rng)
{
  species focal_species = p.extract_random_both_species(rng);
  //const int old_species_in_clade{p.count_species(focal_species.get_clade())};

  p.add_species_mainland(focal_species);

  //const int new_species_in_clade{p.count_species(focal_species.get_clade())};
  //assert(new_species_in_clade == old_species_in_clade - 1);
}

void elly::both_extinction_mainland(populations& p, const double /* time */, std::mt19937& rng)
{
  species focal_species = p.extract_random_both_species(rng);
  p.add_species_island(focal_species);
}

void elly::both_anagenesis(populations& p, const double time, std::mt19937& rng)
{
  species focal_species = p.extract_random_both_species(rng);

  p.add_species_mainland(focal_species);

  const species derived(
    create_new_species_id(),
    focal_species.get_species_id(), //parent_id
    focal_species.get_clade_id(), //clade_id
    time, //time of birth
    location::island
  );
  p.add_species_island(derived);
}

void elly::both_cladogenesis_island(populations& p, const double time, std::mt19937& rng)
{
  species focal_species = p.extract_random_both_species(rng);

  //const int old_species_in_clade{p.count_species(focal_species.get_clade())};

  p.add_species_mainland(focal_species);

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
  p.add_species_island(derived_a);

  const species derived_b(
        create_new_species_id(),
        focal_species.get_species_id(), //parent_id
        focal_species.get_clade_id(), //clade_id
        time, //time of birth
        location::island
  );
  p.add_species_island(derived_b);
}

void elly::both_cladogenesis_mainland(populations& p, const double time, std::mt19937& rng)
{
  species focal_species = p.extract_random_both_species(rng);

  p.add_species_island(focal_species);

  //Give birth to two new lineages
  const species derived_a(
        create_new_species_id(),
        focal_species.get_species_id(), //parent_id
        focal_species.get_clade_id(), //clade_id
        time, //time of birth
        location::mainland
  );
  p.add_species_mainland(derived_a);

  const species derived_b(
        create_new_species_id(),
        focal_species.get_species_id(), //parent_id
        focal_species.get_clade_id(), //clade_id
        time, //time of birth
        location::mainland
  );
  p.add_species_mainland(derived_b);
}
