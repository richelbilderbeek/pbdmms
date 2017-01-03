#include "elly_species.h"
#include "elly_simulation.h"
#include "elly_location.h"

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
    m_time_of_birth{time_of_birth},
    m_time_of_colonization{-1.0},
    m_time_of_extinction_main{-1.0},
    m_time_of_extinction_is{-1.0}
{
  if(m_time_of_birth < 0.0 )
  {
    throw std::logic_error("time of birth or extinction must be larger than 0");
  }
  if (m_location_of_birth == location::both)
  {
    throw std::logic_error("Species cannot be born a cosmopolitan");
  }
  if(m_location_of_birth == location::mainland_only)
  {
    throw std::logic_error("Species cannot be born labeled mainland only");
  }
  if(m_location_of_birth == location::island_only)
  {
    throw std::logic_error("Species cannot be born labeled island only");
  }
}

double elly::get_t_birth_mainland(const species& s) noexcept
{
  assert(s.get_location_of_birth() != location::both);

  if(s.get_location_of_birth() == location::island)
    return -1.0;
  if(s.get_location_of_birth() == location::mainland)
    return s.get_time_of_birth();
  else return 1;
}

double elly::get_t_birth_island(const species& s) noexcept
{
  assert(s.get_location_of_birth() != location::both);

  if(s.get_location_of_birth() == location::island)
    return s.get_time_of_birth();
  if(s.get_location_of_birth() == location::mainland)
    return -1.0;
  else return 1;
}

double elly::get_t_ext_mainland(const species& s) noexcept
{
  return s.get_time_of_extinction_mainland();
}

double elly::get_t_ext_island(const species& s) noexcept
{
  return s.get_time_of_extinction_island();
}

double elly::get_t_colonization(const species& s) noexcept
{
  assert(s.get_time_of_colonization() > 0.0);
  return s.get_time_of_colonization();
}

void elly::species::go_extinct(
  const double time_of_extinction,
  const location location_of_extinction
)
{
  if(location_of_extinction == location::mainland)
    m_time_of_extinction_main = time_of_extinction;
  if(location_of_extinction == location::island)
    m_time_of_extinction_is = time_of_extinction;
}

void elly::species::migrate_to_island(const double colonization_time)
{
  m_time_of_colonization = colonization_time;
  if(m_time_of_extinction_is > 0.0)
    m_time_of_extinction_is = -1.0;
}

void elly::species::set_time_of_colonisation(const double time_of_colonization)
{
  //Can only set extinction time once
  if (m_time_of_colonization != -1.0)
  {
    //Recolonization
    std::clog << "Warning: recolonization of species #" << m_species_id
      << " at time " << time_of_colonization
      << ", keeping newer time of "
      << time_of_colonization << '\n'
    ;
    m_time_of_colonization = time_of_colonization;
    return;
  }
  assert(m_time_of_colonization == -1.0);
  if (time_of_colonization < 0.0)
  {
    throw std::logic_error("time of colonization must be positive");
  }
  m_time_of_colonization = time_of_colonization;
}

void elly::species::set_time_of_extinction(const double time_of_extinction, const location place)
{
  if (time_of_extinction < 0.0)
  {
    throw std::logic_error("time of extinction must be positive");
  }
  if(place == location::mainland)
      m_time_of_extinction_main = time_of_extinction;
  if(place == location::island)
      m_time_of_extinction_is = time_of_extinction;
}

bool elly::is_extant(const species& s) noexcept
{
  if(s.get_location_of_birth() == location::mainland)
    {
      return ((s.get_time_of_extinction_mainland() > 0.0 && s.get_time_of_colonization() > 0.0) ||
              (s.get_time_of_extinction_mainland() > 0.0 && s.get_time_of_extinction_island() < 0.0));
    }
  if(s.get_location_of_birth() == location::island)
    {
      //assuming no migration from island to mainland
      return (s.get_time_of_extinction_island() > 0.0);
    }
  else return true;
}

bool elly::is_extinct(const species& s) noexcept
{
  return !is_extant(s);
}

bool elly::is_on_island(const species& s) noexcept
{
  //Colonization from mainland that still lives
  if(s.get_location_of_birth() == location::mainland)
  {
    return s.get_time_of_colonization() >= 0.0
      && s.get_time_of_extinction_island() == -1.0
    ;
  }
  //Island born species that still lives
  assert(s.get_location_of_birth() == location::island);
  //No migration to mainland
  return s.get_time_of_extinction_island() != -1.0;
}
bool elly::is_on_island_only(const species& s) noexcept
{
  return is_on_island(s) && !is_on_mainland(s);
}

bool elly::is_on_mainland(const species& s) noexcept
{
  return s.get_location_of_birth() == location::mainland
    && s.get_time_of_extinction_mainland() != -1.0
  ;
}

bool elly::is_on_mainland_only(const species& s) noexcept
{
  return is_on_mainland(s) && !is_on_island(s);
}

elly::species elly::create_new_test_species(
  const double time_of_birth,
  const location location_of_birth
)
{
  if (location_of_birth == location::both)
  {
    species s = create_new_test_species(time_of_birth, location::mainland);
    s.migrate_to_island(time_of_birth);
    return s;
  }
  return species(
    create_new_species_id(),
    create_null_species_id(),
    create_new_clade_id(),
    time_of_birth,
    location_of_birth
  );
}

elly::species elly::create_new_test_species(
  const location location_of_birth
)
{
  return create_new_test_species(0.0, location_of_birth);
}

bool elly::operator==(const species& lhs, const species& rhs) noexcept
{
  return
       lhs.get_clade_id() == rhs.get_clade_id()
    && lhs.get_location_of_birth() == rhs.get_location_of_birth()
    && lhs.get_parent_id() == rhs.get_parent_id()
    && lhs.get_species_id() == rhs.get_species_id()
    && lhs.get_time_of_birth() == rhs.get_time_of_birth()
    && lhs.get_time_of_colonization() == rhs.get_time_of_colonization()
    && lhs.get_time_of_extinction_mainland() == rhs.get_time_of_extinction_mainland()
    && lhs.get_time_of_extinction_island() == rhs.get_time_of_extinction_island()
  ;
}

bool elly::operator!=(const species& lhs, const species& rhs) noexcept
{
  return !(lhs == rhs);
}

bool elly::operator<(const species& lhs, const species& rhs) noexcept
{
  return lhs.get_species_id() < rhs.get_species_id();
}
