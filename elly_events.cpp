#include "elly_events.h"
#include "elly_species.h"
#include "elly_parameters.h"
#include "elly_event_rates.h"
#include "elly_simulation.h"

#include <cassert>
#include <random>
#include <cstdlib>

void elly::mainland_cladogenesis(
  simulation& s,
  const double time
)
{
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
  std::vector<species> mainland_species,
  std::vector<species> extinct_species,
  elly::parameters p,
  const double time
)
{
  std::uniform_int_distribution<int> pick_species(0, mainland_species.size());
  int n = pick_species(rng);

  species target = mainland_species[n];
  mainland_species[n] = mainland_species.back();
  mainland_species.pop_back();

  target.set_time_of_extinction(time);
  extinct_species.push_back(target);
}

void elly::mainland_immigration(std::vector<species> mainland_species,
                          std::vector<species> both_species,
                          elly::parameters p,
                          std::vector<int> species_in_clades)
{


  std::uniform_int_distribution<int> pick_species(0, mainland_species.size());
  int n = pick_species(rng);

  species target = mainland_species[n];
  mainland_species[n] = mainland_species.back();
  mainland_species.pop_back();

  both_species.push_back(target);

  int c = target.get_clade();
  species_in_clades[c] += 1;
}

void elly::island_extinction(std::vector<species> island_species,
                       std::vector<species> extinct_species,
                       elly::parameters p,
                       const double time,
                       std::vector<int> species_in_clades)
{


  std::uniform_int_distribution<int> pick_species(0, island_species.size());
  int n = pick_species(rng);

  species target = island_species[n];
  island_species[n] = island_species.back();
  island_species.pop_back();

  target.set_time_of_extinction(time);
  extinct_species.push_back(target);

  int c = target.get_clade();
  species_in_clades[c] -= 1;
}

void elly::island_cladogenesis(
  std::vector<species>& island_species,
  std::vector<species>& extinct_species,
  const parameters& p,
  const double time,
  std::vector<species_id>& species_in_clades
)
{
  std::uniform_int_distribution<int> pick_species(0, island_species.size());
  int n = pick_species(rng);

  species target = island_species[n];
  island_species[n] = island_species.back();
  island_species.pop_back();

  target.set_time_of_extinction(time);
  extinct_species.push_back(target);

  int c = target.get_clade();
  species_in_clades[c] += 1;

  elly::create_species(island_species,
                       target.get_species_id(),
                       time ,
                       id_counter,
                       c);
  elly::create_species(island_species,
                       target.get_species_id(),
                       time ,
                       id_counter,
                       c);
}

void elly::island_immigration(std::vector<species> island_species,
                        std::vector<species> both_species,
                        elly::parameters p)
{


  std::uniform_int_distribution<int> pick_species(0, island_species.size());
  int n = pick_species(rng);

  species target = island_species[n];
  island_species[n] = island_species.back();
  island_species.pop_back();

  both_species.push_back(target);
}

void elly::both_extinction_island(std::vector<species> both_species,
                            std::vector<species> mainland_species,
                            elly::parameters p,
                            std::vector<int> species_in_clades)
{


  std::uniform_int_distribution<int> pick_species(0, both_species.size());
  int n = pick_species(rng);

  species target = both_species[n];
  both_species[n] = both_species.back();
  both_species.pop_back();

  mainland_species.push_back(target);

  int c = target.get_clade();
  species_in_clades[c] -= 1;
}

void elly::both_extinction_mainland(std::vector<species> both_species,
                              std::vector<species> island_species,
                              elly::parameters p)
{


 std::uniform_int_distribution<int> pick_species(0, both_species.size());
 int n = pick_species(rng);

 species target = both_species[n];
 both_species[n] = both_species.back();
 both_species.pop_back();

 island_species.push_back(target);
 }

void elly::both_anagenesis(std::vector<species> mainland_species,
                     std::vector<species> island_species,
                     std::vector<species> both_species,
                     elly::parameters p,
                     const double time,
                     int& id_counter)
{


  std::uniform_int_distribution<int> pick_species(0, both_species.size());
  int n = pick_species(rng);

  species target = both_species[n];
  both_species[n] = both_species.back();
  both_species.pop_back();

  mainland_species.push_back(target);

  elly::create_species(island_species,
                       target.get_species_id(),
                       time, id_counter,
                       target.get_clade());
}

void elly::both_cladogenesis_island(std::vector<species> mainland_species,
                              std::vector<species> island_species,
                              std::vector<species> both_species,
                              elly::parameters p,
                              const double time,
                              int& id_counter,
                              std::vector<int> species_in_clades)
{


  std::uniform_int_distribution<int> pick_species(0, both_species.size());
  int n = pick_species(rng);

  species target = both_species[n];
  both_species[n] = both_species.back();
  both_species.pop_back();

  mainland_species.push_back(target);

  int c = target.get_clade();
  species_in_clades[c] += 1;

  elly::create_species(island_species,
                       target.get_species_id(),
                       time, id_counter,
                       target.get_clade());
  elly::create_species(island_species,
                       target.get_species_id(),
                       time, id_counter,
                       target.get_clade());
}


void elly::both_cladogenesis_mainland(std::vector<species> mainland_species,
                                std::vector<species> island_species,
                                std::vector<species> both_species,
                                elly::parameters p,
                                const double time,
                                int& id_counter)
{


  std::uniform_int_distribution<int> pick_species(0, both_species.size());
  int n = pick_species(rng);

  species target = both_species[n];
  both_species[n] = both_species.back();
  both_species.pop_back();

  island_species.push_back(target);

  elly::create_species(mainland_species,
                       target.get_species_id(),
                       time, id_counter,
                       target.get_clade());
  elly::create_species(mainland_species,
                       target.get_species_id(),
                       time, id_counter,
                       target.get_clade());
}
