#ifndef ELLY_SIMULATION_H
#define ELLY_SIMULATION_H

#include <iosfwd>
#include <random>
#include <vector>
#include "elly_fwd.h"
#include "elly_parameters.h"
#include "elly_populations.h"

namespace elly {

class simulation
{
public:
  simulation(const parameters& p);

  void add_extinct_species(const species& s) { m_populations.add_extinct_species(s); }
  void add_species_mainland(const species& s) { m_populations.add_species_mainland(s); }
  void add_species_island(const species& s) { m_populations.add_species_island(s); }
  void add_species_both(const species& s) { m_populations.add_species_both(s); }

  ///Count the number of species that only occur on a location
  int count_species(const location where) const noexcept { return m_populations.count_species(where); }

  ///Count the number of species in a certain clade
  int count_species(const clade_id& id) const noexcept { return m_populations.count_species(id); }

  ///Gets and removes a random species present in both locations
  ///It is up to the client to put it someplace else
  species extract_random_both_species() { return m_populations.extract_random_both_species(m_rng); }

  ///Gets and removes a random species from the island
  ///It is up to the client to put it someplace else
  species extract_random_island_species() { return m_populations.extract_random_island_species(m_rng); }

  ///Gets and removes a random species from the mainland
  ///It is up to the client to put it someplace else
  species extract_random_mainland_species() { return m_populations.extract_random_mainland_species(m_rng); }

  double get_time() const noexcept { return m_t; }

  std::mt19937& get_rng() noexcept { return m_rng; }

  void run();

  private:

  ///The simulation parameters
  const parameters m_parameters;

  ///The populations
  populations m_populations;

  ///Random number generator
  std::mt19937 m_rng;

  ///The time of the simulation, in million years
  double m_t;
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
