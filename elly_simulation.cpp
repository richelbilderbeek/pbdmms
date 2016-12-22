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
  const double time{s.get_time()};
  species target = s.extract_random_mainland_species();
  target.set_time_of_extinction(time);

  s.add_extinct_species(target);
}

void elly::mainland_immigration(simulation& s)
{
  //ELLY TODO: forward this function to elly_populations.cpp
  const double time{s.get_time()};
  species target = s.extract_random_mainland_species();
  const int old_species_in_clade{s.count_species(target.get_clade())};

  target.set_time_of_colonisation(time);
  s.add_species_both(target);

  //const int c = target.get_clade();
  //species_in_clades[c] += 1;
  const int new_species_in_clade{s.count_species(target.get_clade())};
  assert(new_species_in_clade == old_species_in_clade + 1);
}

void elly::island_extinction(simulation& s)
{
  //ELLY TODO: forward this function to elly_populations.cpp
  const double time{s.get_time()};
  species target = s.extract_random_island_species();
  const int old_species_in_clade{s.count_species(target.get_clade())};

  target.set_time_of_extinction(time);
  s.add_extinct_species(target);

  //int c = target.get_clade();
  //species_in_clades[c] -= 1;
  const int new_species_in_clade{s.count_species(target.get_clade())};
  assert(new_species_in_clade == old_species_in_clade - 1);
}

void elly::island_cladogenesis(simulation& s)
{
  //ELLY TODO: forward this function to elly_populations.cpp
  const double time{s.get_time()};
  species focal_species = s.extract_random_island_species();

  const int old_species_in_clade{s.count_species(focal_species.get_clade())};

  focal_species.set_time_of_extinction(time);
  s.add_extinct_species(focal_species);

  const int new_species_in_clade{s.count_species(focal_species.get_clade())};
  assert(new_species_in_clade == old_species_in_clade - 1);
  //int c = target.get_clade();
  //species_in_clades[c] += 1;

  //Give birth to two new lineages
  const species derived_a(
    time, //time of birth
    focal_species.get_species_id(), //parent_id
    create_new_species_id(),
    focal_species.get_clade() //clade_id
  );
  s.add_species_island(derived_a);

  const species derived_b(
    time, //time of birth
    focal_species.get_species_id(), //parent_id
    create_new_species_id(),
    focal_species.get_clade() //clade_id
  );
  s.add_species_island(derived_b);
}

void elly::both_extinction_island(simulation &s)
{
  //ELLY TODO: forward this function to elly_populations.cpp

  species target = s.extract_random_both_species();

  const int old_species_in_clade{s.count_species(target.get_clade())};

  s.add_species_mainland(target);

  //int c = target.get_clade();
  //species_in_clades[c] -= 1;
  const int new_species_in_clade{s.count_species(target.get_clade())};
  assert(new_species_in_clade == old_species_in_clade - 1);
}

void elly::both_extinction_mainland(simulation& s)
{
  //ELLY TODO: forward this function to elly_populations.cpp
  species target = s.extract_random_both_species();

  s.add_species_island(target);
}

void elly::both_anagenesis(simulation &s)
{
  //ELLY TODO: forward this function to elly_populations.cpp
  const double time{s.get_time()};
  species focal_species = s.extract_random_both_species();

  s.add_species_mainland(focal_species);

  const species derived(
    time, //time of birth
    focal_species.get_species_id(), //parent_id
    create_new_species_id(),
    focal_species.get_clade() //clade_id
  );
  s.add_species_island(derived);
}

void elly::both_cladogenesis_island(simulation &s)
{
  //ELLY TODO: forward this function to elly_populations.cpp
  const double time{s.get_time()};
  species focal_species = s.extract_random_both_species();

  const int old_species_in_clade{s.count_species(focal_species.get_clade())};

  s.add_species_mainland(focal_species);

  const int new_species_in_clade{s.count_species(focal_species.get_clade())};
  assert(new_species_in_clade == old_species_in_clade + 1);
  //int c = focal_species.get_clade();
  //species_in_clades[c] += 1;

  //Give birth to two new lineages
  const species derived_a(
    time, //time of birth
    focal_species.get_species_id(), //parent_id
    create_new_species_id(),
    focal_species.get_clade() //clade_id
  );
  s.add_species_island(derived_a);

  const species derived_b(
    time, //time of birth
    focal_species.get_species_id(), //parent_id
    create_new_species_id(),
    focal_species.get_clade() //clade_id
  );
  s.add_species_island(derived_b);
}


void elly::both_cladogenesis_mainland(simulation &s)
{
  //ELLY TODO: forward this function to elly_populations.cpp
  const double time{s.get_time()};
  species focal_species = s.extract_random_both_species();

  s.add_species_island(focal_species);

  //Give birth to two new lineages
  const species derived_a(
    time, //time of birth
    focal_species.get_species_id(), //parent_id
    create_new_species_id(),
    focal_species.get_clade() //clade_id
  );
  s.add_species_mainland(derived_a);

  const species derived_b(
    time, //time of birth
    focal_species.get_species_id(), //parent_id
    create_new_species_id(),
    focal_species.get_clade() //clade_id
  );
  s.add_species_mainland(derived_b);
}
