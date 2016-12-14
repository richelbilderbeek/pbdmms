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
};

} //~namespace elly
#endif // ELLY_VARIABLES_H
