#include "elly_events.h"
#include "elly_species.h"
#include "elly_parameters.h"
#include "elly_event_rates.h"
#include "elly_simulation.h"

#include <cassert>
#include <random>
#include <cstdlib>

void elly::mainland_cladogenesis(simulation& s)
{
  const double time{s.get_time()};
  species focal_species = s.extract_random_mainland_species();

  //Make that focal species go extict
  focal_species.set_time_of_extinction(time);
  s.add_extinct_species(focal_species);

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

void elly::mainland_extinction(
  simulation& s
)
{
  const double time{s.get_time()};
  species target = s.extract_random_mainland_species();
  target.set_time_of_extinction(time);

  s.add_extinct_species(target);
}

void elly::mainland_immigration(simulation& s)
{
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

#ifdef ALLOW_COLONIZATION_OF_MAINLAND_FROM_ISLAND
void elly::island_immigration(
  simulation& s,
  const double time
)
{
  species target = s.extract_random_island_species();
  //target.set_time_of_mainland_colonisation(time);
  both_species.push_back(target);
}
#endif // ALLOW_COLONIZATION_OF_MAINLAND_FROM_ISLAND

void elly::both_extinction_island(simulation &s)
{
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
  species target = s.extract_random_both_species();

  s.add_species_island(target);
}

void elly::both_anagenesis(simulation &s)
{
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
