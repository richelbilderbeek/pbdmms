#ifndef ELLY_SIMULATION_H
#define ELLY_SIMULATION_H

#include <iostream>
#include <vector>
#include <random>
#include "elly_species.h"
#include "elly_parameters.h"

namespace elly {

class simulation
{
public:
  simulation(const parameters& p);

  ///Gets and removes a random species from the mainland
  ///It is up to the client to put it someplace else
  species extract_random_mainland_species();

  void run();

  private:

  std::vector<species> m_species_mainland;
  std::vector<species> m_species_island;
  std::vector<species> m_species_both;
  std::vector<species> m_extinct_species;

  const parameters m_parameters;

  ///Random number generator
  std::mt19937 m_rng;

  //adding species to species vector
  void add_extinct_species(const species& s);
  void add_species_mainland(const species& s);
  void add_species_island(const species& s);
  void add_species_both(const species& s);

  //removing species from vector at position i
  void remove_species_mainland(const int i);
  void remove_species_island(const int i);
  void remove_species_both(const int i);
};

//creating a new species, taking
/*
void create_mainland_species(
  const simulation& v,
  const int parent_id ,
  const double time,
  int& id_counter,
  int clade
) noexcept;
*/

} //~namespace elly

#endif // ELLY_SIMULATION_H
