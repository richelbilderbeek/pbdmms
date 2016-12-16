#ifndef SPECIES_H
#define SPECIES_H
#include "elly_variables.h"
#include <iostream>
#include <stdexcept>
#include <vector>

namespace elly {

class species
{
public:
  species(
    double time_of_birth = 0.0,
    int parent_id = 0,
    int species_id = 0,
    double time_of_extinction = -1.0,
    int clade = 0
  );

  void iterate_species_id();
  int get_species_id() const noexcept {  return m_species_id; }
  int get_parent_id() const noexcept {  return m_parent_id;  }
  double get_time_of_birth() const noexcept {  return m_time_of_birth; }
  double get_time_of_extinction() const noexcept {  return m_time_of_extinction; }
  int get_clade() const noexcept { return m_clade; }
  void set_species_id(const int species_id);
  void set_parent_id(const int parent_id);
  void set_time_of_birth(const double time_of_birth);
  void set_time_of_extinction(const double time_of_extinction);
  void set_clade( const int clade);

private:
  double m_time_of_birth;
  //time of birth of species

  int    m_parent_id;
  //integer corresponding to parentspecies

  int    m_species_id;
  //integer corresponding to this certain species

  double m_time_of_extinction;
  //time of extinction, when species is not extinct it is equal to -1

  int m_clade;
  //integer corresponding to clade of species
};

//creating a new species, taking
void create_species(variables v,
                    const int parent_id ,
                    const double time,
                    int& id_counter,
                    int clade) noexcept;

} //~namespace elly
#endif // SPECIES_H
