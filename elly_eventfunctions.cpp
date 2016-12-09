#include "elly_eventfunctions.h"
#include "elly_species.h"
#include "elly_parameters.h"
#include <random>
#include <cstdlib>

void mainland_cladogenesis(std::vector<elly::species> mainland_species,
                           std::vector<elly::species> extinct_species,
                           elly::parameters p,
                           int id_counter,
                           const double time)
{


  std::mt19937_64 rng;
  rng.seed(p.get_rng_seed());
  std::uniform_int_distribution<int> pick_species(0, mainland_species.size());
  int n = pick_species(rng);

  elly::species target = mainland_species[n];
  mainland_species[n] = mainland_species.back();
  mainland_species.pop_back();

  target.set_time_of_extinction(time);
  extinct_species.push_back(target);

  int c = target.get_clade();

  elly::create_species(mainland_species,
                       target.get_species_id(),
                       time ,
                       id_counter,
                       c);
  elly::create_species(mainland_species,
                       target.get_species_id(),
                       time ,
                       id_counter,
                       c);
}

void mainland_extinction(std::vector<elly::species> mainland_species,
                         std::vector<elly::species> extinct_species,
                         elly::parameters p,
                         const double time)
{
  std::mt19937_64 rng;
  rng.seed(p.get_rng_seed());
  std::uniform_int_distribution<int> pick_species(0, mainland_species.size());
  int n = pick_species(rng);

  elly::species target = mainland_species[n];
  mainland_species[n] = mainland_species.back();
  mainland_species.pop_back();

  target.set_time_of_extinction(time);
  extinct_species.push_back(target);
}

void mainland_immigration(std::vector<elly::species> mainland_species,
                          std::vector<elly::species> both_species,
                          elly::parameters p,
                          std::vector<int> species_in_clade)
{
  std::mt19937_64 rng;
  rng.seed(p.get_rng_seed());
  std::uniform_int_distribution<int> pick_species(0, mainland_species.size());
  int n = pick_species(rng);

  elly::species target = mainland_species[n];
  mainland_species[n] = mainland_species.back();
  mainland_species.pop_back();

  both_species.push_back(target);

  int c = target.get_clade();
  species_in_clade[c] += 1;
}

void island_extinction(std::vector<elly::species> island_species,
                       std::vector<elly::species> extinct_species,
                       elly::parameters p,
                       const double time,
                       std::vector<int> clade_counter)
{
  std::mt19937_64 rng;
  rng.seed(p.get_rng_seed());
  std::uniform_int_distribution<int> pick_species(0, island_species.size());
  int n = pick_species(rng);

  elly::species target = island_species[n];
  island_species[n] = island_species.back();
  island_species.pop_back();

  target.set_time_of_extinction(time);
  extinct_species.push_back(target);

  int c = target.get_clade();
  clade_counter[c] -= 1;
}

void island_cladogenesis(std::vector<elly::species> island_species,
                         std::vector<elly::species> extinct_species,
                         elly::parameters p,
                         int id_counter,
                         const double time,
                         std::vector<int> clade_counter)
{
  std::mt19937_64 rng;
  rng.seed(p.get_rng_seed());
  std::uniform_int_distribution<int> pick_species(0, island_species.size());
  int n = pick_species(rng);

  elly::species target = island_species[n];
  island_species[n] = island_species.back();
  island_species.pop_back();

  target.set_time_of_extinction(time);
  extinct_species.push_back(target);

  int c = target.get_clade();
  clade_counter[c] += 1;

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

void island_immigration();
void both_extinction_island();
void both_extinction_mainland();
void both_anagenesis();
void both_cladogenesis_island();
void both_cladogenesis_mainland();
