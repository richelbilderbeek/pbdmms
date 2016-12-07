#include "elly_individual.h"
#include <stdexcept>

elly::individual::individual(
    double time_of_birth,
    int parent_id,
    int species_id,
    double time_of_extinction,
    int id_counter
)
  : m_time_of_birth{time_of_birth},
    m_parent_id{parent_id},
    m_species_id{species_id},
    m_time_of_extinction{time_of_extinction},
    m_id_counter{id_counter}
{
 if(time_of_birth < 0.0 || time_of_extinction < 0.0)
   throw std::logic_error("time of birth or extinction must be larger than 0");
 if(parent_id < 0 || species_id < 0)
     throw std::logic_error("species id must be larger than 0");
}

  void elly::individual::iterate_species_id()
  {
    m_id_counter += 1;
  }
