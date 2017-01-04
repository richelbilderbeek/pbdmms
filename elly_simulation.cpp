#include "elly_simulation.h"

#include <cassert>
#include <iostream>
#include <cstdlib>
#include <exception>
#include <vector>

#include "elly_location.h"
#include "elly_clade_id.h"
#include "elly_species.h"
#include "elly_species_id.h"
#include "elly_event_rates.h"
#include "elly_gillespie.h"

elly::simulation::simulation(
  const parameters& p
)
  : m_parameters{p},
    m_populations(p),
    m_rng(p.get_rng_seed()),
    m_t{0.0}
{

}

int elly::simulation::count_species(const location where) const noexcept
{
  return m_populations.count_species(where);
}

int elly::simulation::count_species(const clade_id& id) const noexcept
{
  return m_populations.count_species(id);
}

void elly::simulation::do_next_event()
{
  const event_rates r(m_parameters, *this);
  m_t += draw_waiting_time(r, m_rng);
  do_event(r, *this);
}

elly::species elly::simulation::extract_random_species(const location any_location)
{
  return m_populations.extract_random_species(any_location, m_rng);
}

void elly::simulation::run()
{
  //Initial populations are already initialized
  assert(m_parameters.get_init_n_mainland()
    == m_populations.count_species(location::mainland_only)
  );

  const double t_end{m_parameters.get_crown_age()};
  while (m_t < t_end)
  {
    do_next_event();
  }
}

void elly::cladogenesis_mainland_only(simulation& s)
{
  cladogenesis_mainland_only(s.get_populations(), s.get_time(), s.get_rng());
}

void elly::mainland_extinction(simulation& s)
{
  mainland_extinction(s.get_populations(), s.get_time(), s.get_rng());
}

void elly::mainland_immigration(simulation& s)
{
  mainland_immigration(s.get_populations(), s.get_time(), s.get_rng());
}

void elly::island_extinction(simulation& s)
{
  island_extinction(s.get_populations(), s.get_time(), s.get_rng());
}

void elly::cladogenesis_island_only(simulation& s)
{
  cladogenesis_island_only(s.get_populations(), s.get_time(), s.get_rng());
}

void elly::both_extinction_island(simulation &s)
{
  both_extinction_island(s.get_populations(), s.get_time(), s.get_rng());
}

void elly::both_extinction_mainland(simulation& s)
{
  both_extinction_mainland(s.get_populations(), s.get_time(), s.get_rng());
}

void elly::both_anagenesis(simulation &s)
{
  both_anagenesis(s.get_populations(), s.get_time(), s.get_rng());
}

void elly::cladogenesis_global_on_island(simulation &s)
{
  cladogenesis_global_on_island(s.get_populations(), s.get_time(), s.get_rng());
}

void elly::cladogenesis_global_on_mainland(simulation &s)
{
  cladogenesis_global_on_mainland(s.get_populations(), s.get_time(), s.get_rng());
}
