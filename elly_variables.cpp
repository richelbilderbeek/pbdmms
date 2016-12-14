#include <iostream>
#include <cstdlib>
#include <exception>
#include <vector>
#include "elly_variables.h"

elly::variables::variables(
    double time,
    int main_init)
  : m_species_in_clades(main_init),
    m_time{0.0},
    m_main_init{500},
    m_id_counter{0},
    m_all_species_mainland(main_init),
    m_all_species_island{},
    m_all_species_both{}
{
  if(time < 0.0)
    throw std::invalid_argument("time can not be negative");
}
