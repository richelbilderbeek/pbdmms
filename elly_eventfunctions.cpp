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

  extinct_species.push_back(target);

  elly::create_species(mainland_species, target.get_species_id() , time , id_counter);
}

void mainland_extinction()
{

}

void mainland_immigration();
void island_extinction();
void island_cladogenesis();
void island_immigration();
void both_extinction_island();
void both_extinction_mainland();
void both_anagenesis();
void both_cladogenesis_island();
void both_cladogenesis_mainland();
