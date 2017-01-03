#include "elly_species.h"
#include "elly_simulation.h"

#include <cassert>
#include <stdexcept>

elly::species::species(
    const species_id this_species_id,
    const species_id parent_id,
    const clade_id this_clade_id,
    const double time_of_birth,
    const location location_of_birth
)
  :
    m_clade_id{this_clade_id},
    m_location_of_birth{location_of_birth},
    m_parent_id{parent_id},
    m_species_id{this_species_id},
    m_time_of_birth{time_of_birth}
{
  if(m_time_of_birth < 0.0 )
  {
    throw std::logic_error("time of birth or extinction must be larger than 0");
  }
  if (m_location_of_birth == location::both)
  {
    throw std::logic_error("Species cannot be born a cosmopolitan");
  }
}

/*
void elly::species::set_time_of_colonisation(const double time_of_colonization)
{
  //Can only set extinction time once
  if (m_time_of_colonization != -1.0)
  {
    //Recolonization
    std::clog << "Warning: recolonization of species #" << m_species_id
      << " at time " << time_of_colonization
      << ", keeping older time of "
      << m_time_of_colonization << '\n'
    ;
    return;
  }
  assert(m_time_of_colonization == -1.0);
  if (time_of_colonization < 0.0)
  {
    throw std::logic_error("time of colonization must be positive");
  }
  m_time_of_colonization = time_of_colonization;
}

void elly::species::set_time_of_extinction(const double time_of_extinction)
{
  //Can only set extinction time once
  assert(m_time_of_extinction == -1.0);
  if (time_of_extinction < 0.0)
  {
    throw std::logic_error("time of extinction must be positive");
  }
  m_time_of_extinction = time_of_extinction;
}

bool elly::is_extant(const species& s) noexcept
{
  return s.get_time_of_extinction() < 0.0;
}

bool elly::is_extinct(const species& s) noexcept
{
  return !is_extant(s);
}
*/
