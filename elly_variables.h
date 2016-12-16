#ifndef ELLY_VARIABLES_H
#define ELLY_VARIABLES_H

#include <iostream>
#include <vector>
#include "elly_species.h"

namespace elly {

class variables
{
public:
  variables(
      double time = 0.0,
      int main_init = 500
    );

    std::vector<int> m_species_in_clades;
    double m_time;
    int m_main_init;
    int m_id_counter;
    std::vector<species> m_all_species_mainland;
    std::vector<species> m_all_species_island;
    std::vector<species> m_all_species_both;
    std::vector<species> m_extinct_species;
    int m_mo;
    int m_io;
    int m_bo;

    //functions to retrieve member functions of class variables
    double get_time() const noexcept {return m_time; }
    int get_id_counter() const noexcept { return m_id_counter; }
    int get_main_init() const noexcept { return m_main_init;  }

    //update time by adding waiting time
    void iterate_time(const double waiting_time);

    //updating id_counter
    void iterate_id_counter(){ m_id_counter += 1;}

    //adding species to species vector
    void add_species_mainland(const species newspecies);
    void add_species_island(const species newspecies);
    void add_species_both(const species newspecies);

    //removing species from vector at position i
    void remove_species_mainland(const int i);
    void remove_species_island(const int i);
    void remove_species_both(const int i);
};

} //~namespace elly
#endif // ELLY_VARIABLES_H
