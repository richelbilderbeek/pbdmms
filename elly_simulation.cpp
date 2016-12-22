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

void elly::simulation::run()
{
  //Initial populations are already initialized
  assert(m_parameters.get_init_n_mainland() == m_populations.count_species(location::mainland_only));

  const double t_end{m_parameters.get_crown_age()};
  while (m_t < t_end)
  {
    const event_rates r(m_parameters, *this);

    m_t += draw_waiting_time(r, m_rng);

    do_event(r, *this);
  }
}

void elly::mainland_cladogenesis(simulation& s)
{
  //ELLY TODO: this function has been nicely forwarded to elly_populations.h
  // and implemented in elly_populations.cpp
  mainland_cladogenesis(s.get_populations(), s.get_time(), s.get_rng());
}

void elly::mainland_extinction(
  simulation& s
)
{
  //ELLY TODO: forward this function to elly_populations.cpp
  // the declaratation in elly_populations.h is already present
  mainland_extinction(s.get_populations(), s.get_time(), s.get_rng());
}

void elly::mainland_immigration(simulation& s)
{
  //ELLY TODO: forward this function to elly_populations.cpp
  mainland_immigration(s.get_populations(), s.get_time(), s.get_rng());
}

void elly::island_extinction(simulation& s)
{
  //ELLY TODO: forward this function to elly_populations.cpp
  island_extinction(s.get_population(), s.get_time(), s.get_rng());
}

void elly::island_cladogenesis(simulation& s)
{
  //ELLY TODO: forward this function to elly_populations.cpp
  island_cladogenesis(s.get_population(), s.get_time(), s.get_rng());
}

void elly::both_extinction_island(simulation &s)
{
  //ELLY TODO: forward this function to elly_populations.cpp
  both_extinction_island(s.get_population(), s.get_time(), s.get_rng());
}

void elly::both_extinction_mainland(simulation& s)
{
  //ELLY TODO: forward this function to elly_populations.cpp 
  both_extinction_mainland(s.get_population(), s.get_rng());
}

void elly::both_anagenesis(simulation &s)
{
  //ELLY TODO: forward this function to elly_populations.cpp
  both_anagenesis(s.get_population(), s.get_time(), s.get_rng());
}

void elly::both_cladogenesis_island(simulation &s)
{
  //ELLY TODO: forward this function to elly_populations.cpp
  both_cladogenesis_island(s.get_population(), s.get_time(), s.get_rng());
}

void elly::both_cladogenesis_mainland(simulation &s)
{
  //ELLY TODO: forward this function to elly_populations.cpp
  both_cladogenesis_mainland(s.get_population(), s.get_time(), s.get_rng());
}
