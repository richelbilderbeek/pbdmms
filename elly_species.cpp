#include "elly_species.h"
#include <stdexcept>

elly::species::species(
    double time_of_birth,
    int parent_id,
    int species_id,
    double time_of_extinction,
    int clade
)
  : m_time_of_birth{time_of_birth},
    m_parent_id{parent_id},
    m_species_id{species_id},
    m_time_of_extinction{time_of_extinction},
    m_clade{clade}
{
 if(m_time_of_birth < 0.0 )
   throw std::logic_error("time of birth or extinction must be larger than 0");
 if (m_time_of_extinction < -1)
   throw std::logic_error("time of extinction must be larger than -1");
}

void elly::species::set_species_id(const int species_id)
{
  m_species_id = species_id;
}

void elly::species::set_parent_id(const int parent_id)
{
  m_parent_id = parent_id;
}

void elly::species::set_time_of_birth(const double time_of_birth)
{
  if (time_of_birth < 0.0)
    throw std::logic_error("time of birth must be larger than 0");
  m_time_of_birth = time_of_birth;
}

void elly::species::set_time_of_extinction(const double time_of_extinction)
{
  if (time_of_extinction < 0.0)
    throw std::logic_error("time of extinction must be largen than 0");
  m_time_of_extinction = time_of_extinction;
}

void elly::species::set_clade(const int clade)
{
  m_clade = clade;
}

void elly::create_species(std::vector<species> &v,
                               const int parent_id ,
                               const double time,
                               int& id_counter,
                               const int clade) noexcept
  {
    id_counter += 1;

    elly::species new_indv;
    new_indv.set_time_of_birth(time);
    new_indv.set_parent_id(parent_id);
    new_indv.set_species_id(id_counter);
    new_indv.set_clade(clade);
    v.push_back(new_indv);
  }
