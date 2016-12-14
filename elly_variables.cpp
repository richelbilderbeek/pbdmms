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
    m_all_species_both{}
{
  if(time < 0.0)
    throw std::invalid_argument("time can not be negative");
}


void iterate_time(const double waiting_time)
{
  if(waiting_time < 0.0)
    throw std::invalid_argument("waiting time must be positive");
  m_time += waiting_time;
}

void add_species_mainland(const species newspecies)
{
  m_all_species_mainland.push_back(newspecies);
}

void add_species_island(const species newspecies)
{
  m_all_island_species.push_back(newspecies);
}

void add_species_both(const species newspecies)
{
  m_all_species_both.push_back(newspecies);
}

void remove_species_mainland(const int i){
  elly::species temp =  m_all_species_mainland.end();
  m_all_species_mainland[i] = temp;
  m_all_species_mainland.pop_back();
}

void remove_species_island(const int i)
{
  elly::species temp =  m_all_species_island.end();
  m_all_species_island[i] = temp;
  m_all_species_island.pop_back();
}

void remove_species_both(const int i)
{
  elly::species temp =  m_all_species_both.end();
  m_all_species_both[i] = temp;
  m_all_species_both.pop_back();
}
