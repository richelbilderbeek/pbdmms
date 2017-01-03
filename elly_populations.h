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
  ///Initialize from parameters
  populations(const parameters& p);

  populations(
    std::vector<species> extinct_species,
    std::vector<species> species_both,
    std::vector<species> species_island,
    std::vector<species> species_mainland
  );

  void add_extinct_species(const species& s);
  void add_species_mainland(const species& s);
  void add_species_island(const species& s);
  void add_species_both(const species& s);

  ///Just collect all species, every species present only once
  std::vector<species> collect_all_species() const noexcept;

  ///Count the number of extinct species
  int count_extinct_species() const noexcept;

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
};

std::vector<species> create_initial_mainland_species(const parameters& p);

///Create a population with
/// * 1 island-only species
/// * 1 mainland-only species
/// * 1 global species
/// * 1 extinct species
/// The chronology is
/// * #1 was born on the mainland and is mainland-only
/// * #2 was born on the mainland and colonized the island, thus is global, derived from #1
/// * #3 was born on the island and is island only, derived from #2
/// * #4 like #3, except went extinct
populations create_test_populations_1();

///adds two new mainland species from the same clade as the parent species,
///also pushes parent species from mainland species vector to extinct species vector
void mainland_cladogenesis(populations& p, const double time, std::mt19937& rng);

///pushes random species from mainland species vector to extinct species vector
void mainland_extinction(populations& p, const double time, std::mt19937& rng);

///removes species from mainland species vector and adds it to both (mainland and island) vector
void mainland_immigration(populations& p, const double time, std::mt19937& rng);

///pushes random species from island species vector to extinct species vector
void island_extinction(populations& p, const double time, std::mt19937& rng);

//adds two new island species from the same clade as the parent species,
//also pushes parent species from island species vector to extinct species vector
void island_cladogenesis(populations& p, const double time, std::mt19937& rng);

//removes species from both habitats species vector and adds it to mainland species vector
void both_extinction_island(populations& p, const double time, std::mt19937& rng);

//removes species from both habitats species vector and adds it to island species vector
void both_extinction_mainland(populations& p, const double time, std::mt19937& rng);

//removes species from both habitats species vector and adds it to mainland species vector,
//also creating a new species in island species vector
void both_anagenesis(populations& p, const double time, std::mt19937& rng);

//removes species from both habitats species vector and adds it to mainland species vector,
//also creates two new species in island species vector
void both_cladogenesis_island(populations& p, const double time, std::mt19937& rng);

//removes species from both habitats species vector and adds it to island species vector,
//also creates two new species in mainland species vector
void both_cladogenesis_mainland(populations& p, const double time, std::mt19937& rng);

} //~namespace elly
#endif // ELLY_POPULATIONS_H
