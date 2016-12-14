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

    double get_time() const noexcept {return m_time; }
    int get_id_counter() const noexcept { return m_id_counter; }
    void iterate_time(const double waiting_time);
    void iterate_id_counter(){ m_id_counter += 1;}
    void add_species_mainland(const species newspecies);
    void add_species_island(const species newspecies);
    void add_species_both(const species newspecies);
    void remove_species_mainland(const int i);
    void remove_species_island(const int i);
    void remove_species_both(const int i);
};

} //~namespace elly
#endif // ELLY_VARIABLES_H
