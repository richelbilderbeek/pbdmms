#include <iostream>
#include <cstdlib>
#include <exception>
#include <vector>
#include "elly_variables.h"


elly::variables::variables(
    double time,
    int main_init)
  : m_species_in_clades(main_init),
    m_time{time},
    m_main_init{500},
    m_id_counter{0},
    m_all_species_mainland(main_init),
    m_all_species_island{},
    m_all_species_both{},
    m_extinct_species{},
    m_mo{m_main_init},
    m_io{0},
    m_bo{0}
{
  if(time < 0.0)
    throw std::invalid_argument("time can not be negative");
}

void elly::variables::iterate_time(const double waiting_time)
{
  if(waiting_time < 0.0)
    throw std::invalid_argument("waiting time must be positive");
  m_time += waiting_time;
}

void elly::variables::add_species_mainland(const elly::species newspecies)
{
  m_all_species_mainland.push_back(newspecies);
}

void elly::variables::add_species_island(const elly::species newspecies)
{
  m_all_species_island.push_back(newspecies);
}

void elly::variables::add_species_both(const elly::species newspecies)
{
  m_all_species_both.push_back(newspecies);
}

void elly::variables::remove_species_mainland(const int i){
  elly::species temp =  m_all_species_mainland.back();
  m_all_species_mainland[i] = temp;
  m_all_species_mainland.pop_back();
}

void elly::variables::remove_species_island(const int i)
{
  elly::species temp =  m_all_species_island.back();
  m_all_species_island[i] = temp;
  m_all_species_island.pop_back();
}

void elly::variables::remove_species_both(const int i)
{
  elly::species temp =  m_all_species_both.back();
  m_all_species_both[i] = temp;
  m_all_species_both.pop_back();
}
