#include "elly_simulation.h"

#include <cassert>
#include <iostream>
#include <cstdlib>
#include <exception>
#include <vector>

#include "elly_species_id.h"
#include "elly_event_rates.h"
#include "elly_gillespie.h"

elly::simulation::simulation(
  const parameters& p
)
  : m_species_mainland{},
    m_species_island{},
    m_species_both{},
    m_extinct_species{},
    m_parameters{p},
    m_rng(p.get_rng_seed())
{
  //Create the initial species on the mainland
  {
    const int n{p.get_init_n_mainland()};
    m_species_mainland.reserve(n);
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
      m_species_mainland.push_back(s);
    }
  }
}

void elly::simulation::add_extinct_species(const species& s)
{
  assert(is_extinct(s));
  this->m_extinct_species.push_back(s);
}

void elly::simulation::add_species_mainland(const species& s)
{
  assert(is_extant(s));
  m_species_mainland.push_back(s);
}

void elly::simulation::add_species_island(const species& s)
{
  assert(is_extant(s));
  m_species_island.push_back(s);
}

void elly::simulation::add_species_both(const species& s)
{
  assert(is_extant(s));
  m_species_both.push_back(s);
}

elly::species elly::simulation::extract_random_mainland_species()
{
  std::uniform_int_distribution<int> species_indices(0, m_species_mainland.size());
  const int n = species_indices(rng);

  assert(n >= 0);
  assert(n < static_cast<int>(mainland_species.size()));

  //Extract focal species from mainland, remove it from that vector
  const species s = mainland_species[n];
  mainland_species[n] = mainland_species.back();
  mainland_species.pop_back();
  return s;

}

void elly::simulation::remove_species_mainland(const int i)
{
  assert(i >= 0);
  assert(i < static_cast<int>(m_species_mainland.size()));
  std::swap(m_species_mainland[i], m_species_mainland.back());
  m_species_mainland.pop_back();
}

void elly::simulation::remove_species_island(const int i)
{
  assert(i >= 0);
  assert(i < static_cast<int>(m_species_island.size()));
  std::swap(m_species_island[i], m_species_island.back());
  m_species_island.pop_back();
}

void elly::simulation::remove_species_both(const int i)
{
  assert(i >= 0);
  assert(i < static_cast<int>(m_species_both.size()));
  std::swap(m_species_both[i], m_species_both.back());
  m_species_both.pop_back();
}

void elly::simulation::run()
{
  //Initial populations are already initialized
  assert(m_parameters.get_init_n_mainland() == static_cast<int>(m_species_mainland.size()));
  const double t_end{m_parameters.get_crown_age()};
  double t{0.0};
  while (t < t_end)
  {
    const event_rates r{
      calculate_rates(
        m_parameters,
        *this
      )
    };

    t += draw_waiting_time(r, m_rng);

    const int n{draw_event(r, m_rng)};

    assert(n > 0);
    assert(n < 11);
    do_nth_event(
      n,
      *this,
      t
    );
  }
}

/*
void elly::create_mainland_species(
  simulation& v,
  const int parent_id ,
  const double time,
  int clade
) noexcept
{
  id_counter += 1;

  species s;
  s.set_time_of_birth(time);
  s.set_parent_id(parent_id);
  s.set_species_id(id_counter);
  s.set_clade(clade);
  v.add_species_mainland(s);
}
*/
