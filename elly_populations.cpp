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

int elly::populations::count_species(const location where) const noexcept
{
  switch (where)
  {
    case location::both: return static_cast<int>(this->m_species_both.size());
    case location::island_only: return static_cast<int>(this->m_species_island.size());
    case location::mainland_only: return static_cast<int>(this->m_species_mainland.size());
    case location::island: return count_species(location::island_only) + count_species(location::both);
    case location::mainland: return count_species(location::mainland_only) + count_species(location::both);
  }
  assert(!"Should not get here");
  throw std::logic_error("Should not get here");
}

int elly::populations::count_species(const clade_id& id) const noexcept
{
  assert(id.get_id() == 1);
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
    const double time_of_birth{0.0};
    const species_id parent_id{create_null_species_id()};
    const species_id id{create_new_species_id()};
    const clade_id clade{create_new_clade_id()};
    const species s(
      time_of_birth,
      parent_id,
      id,
      clade
    );
    v.push_back(s);
  }
  return v;
}

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
  return extract_random_species(m_species_both, rng);
}

elly::species elly::populations::extract_random_island_species(std::mt19937& rng)
{
  return extract_random_species(m_species_island, rng);
}

elly::species elly::populations::extract_random_mainland_species(std::mt19937& rng)
{
  return extract_random_species(m_species_mainland, rng);
}

void elly::populations::remove_species_mainland(const int i)
{
  assert(i >= 0);
  assert(i < static_cast<int>(m_species_mainland.size()));
  std::swap(m_species_mainland[i], m_species_mainland.back());
  m_species_mainland.pop_back();
}

void elly::populations::remove_species_island(const int i)
{
  assert(i >= 0);
  assert(i < static_cast<int>(m_species_island.size()));
  std::swap(m_species_island[i], m_species_island.back());
  m_species_island.pop_back();
}

void elly::populations::remove_species_both(const int i)
{
  assert(i >= 0);
  assert(i < static_cast<int>(m_species_both.size()));
  std::swap(m_species_both[i], m_species_both.back());
  m_species_both.pop_back();
}
