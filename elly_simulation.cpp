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
  : m_species_in_clades{},
    m_all_species_mainland{},
    m_all_species_island{},
    m_all_species_both{},
    m_extinct_species{},
    m_mo{p.get_main_init()},
    m_io{0},
    m_bo{0},
    m_parameters{p},
    m_rng(p.get_rng_seed())
{
  //Create the initial species on the mainland
  {
    const int n{p.get_main_init()};
    m_species_in_clades.reserve(n);
    m_all_species_mainland.reserve(n);
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
      m_species_in_clades.push_back(id);
      m_all_species_mainland.push_back(s);
    }
  }
}

void elly::simulation::add_species_mainland(const species& newspecies)
{
  m_all_species_mainland.push_back(newspecies);
}

void elly::simulation::add_species_island(const species& newspecies)
{
  m_all_species_island.push_back(newspecies);
}

void elly::simulation::add_species_both(const species& newspecies)
{
  m_all_species_both.push_back(newspecies);
}

void elly::simulation::remove_species_mainland(const int i)
{
  assert(i >= 0);
  assert(i < static_cast<int>(m_all_species_mainland.size()));
  std::swap(m_all_species_mainland[i], m_all_species_mainland.back());
  m_all_species_mainland.pop_back();
}

void elly::simulation::remove_species_island(const int i)
{
  assert(i >= 0);
  assert(i < static_cast<int>(m_all_species_island.size()));
  std::swap(m_all_species_island[i], m_all_species_island.back());
  m_all_species_island.pop_back();
}

void elly::simulation::remove_species_both(const int i)
{
  assert(i >= 0);
  assert(i < static_cast<int>(m_all_species_both.size()));
  std::swap(m_all_species_both[i], m_all_species_both.back());
  m_all_species_both.pop_back();
}

void elly::simulation::run()
{
  //Initial populations are already initialized
  assert(m_parameters.get_main_init() == static_cast<int>(m_species_in_clades.size()));
  assert(m_parameters.get_main_init() == static_cast<int>(m_all_species_mainland.size()));
  const double t_end{m_parameters.get_crown_age()};
  double t{0.0};
  while (t < t_end)
  {
    const event_rates r{
      calculate_rates(
        m_parameters,
        m_mo,
        m_io,
        m_bo,
        m_species_in_clades
      )
    };

    t += draw_waiting_time(r, m_rng);

    const int n{draw_event(r, m_rng)};

    assert(n > 0);
    assert(n < 11);
    do_nth_event(
      n,
      m_all_species_mainland,
      m_all_species_island,
      m_all_species_both,
      m_parameters,
      m_extinct_species,
      m_species_in_clades,
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
