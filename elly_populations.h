#ifndef ELLY_POPULATIONS_H
#define ELLY_POPULATIONS_H

#include <random>
#include <vector>

#include "elly_species.h"
#include "elly_fwd.h"

namespace elly {

class populations
{
public:
  populations(const parameters& p);

  void add_extinct_species(const species& s);
  void add_species_mainland(const species& s);
  void add_species_island(const species& s);
  void add_species_both(const species& s);

  ///Count the number of species that only occur on a location
  int count_species(const location where) const noexcept;

  ///Count the number of species in a certain clade
  int count_species(const clade_id& id) const noexcept;

  ///Gets and removes a random species present in both locations
  ///It is up to the client to put it someplace else
  species extract_random_both_species(std::mt19937& rng);

  ///Gets and removes a random species from the island
  ///It is up to the client to put it someplace else
  species extract_random_island_species(std::mt19937& rng);

  ///Gets and removes a random species from the mainland
  ///It is up to the client to put it someplace else
  species extract_random_mainland_species(std::mt19937& rng);

private:
  std::vector<species> m_extinct_species;
  std::vector<species> m_species_both;
  std::vector<species> m_species_island;
  std::vector<species> m_species_mainland;


  species extract_random_species(
    std::vector<species>& s,
    std::mt19937& rng
  );

  //removing species from vector at position i
  void remove_species_mainland(const int i);
  void remove_species_island(const int i);
  void remove_species_both(const int i);
};

std::vector<species> create_initial_mainland_species(const parameters& p);

} //~namespace elly
#endif // ELLY_POPULATIONS_H
