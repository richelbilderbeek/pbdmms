#include "elly_species.h"
#include "elly_simulation.h"
#include "elly_location.h"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <iterator>

elly::species::species(const species_id this_species_id,
    const species_id parent_id,
    const clade_id this_clade_id,
    const double time_of_birth,
    const location location_of_birth)
  :
    m_clade_id{this_clade_id},
    m_location_of_birth{location_of_birth},
    m_parent_id{parent_id},
    m_species_id{this_species_id},
    m_time_of_birth{time_of_birth},
    m_times_of_colonization{},
    m_time_of_extinction_main{-1.0},
    m_time_of_extinction_is{-1.0}
{
  if(m_time_of_birth < 0.0 )
  {
    throw std::invalid_argument("time of birth or extinction must be larger than 0");
  }
  if (m_location_of_birth == location::both)
  {
    throw std::invalid_argument("Species cannot be born a cosmopolitan");
  }
  if(m_location_of_birth == location::mainland_only)
  {
    throw std::invalid_argument("Species cannot be born labeled mainland only");
  }
  if(m_location_of_birth == location::island_only)
  {
    throw std::invalid_argument("Species cannot be born labeled island only");
  }
}

elly::species elly::create_descendant(
  const species& parent,
  const double time_of_birth,
  const location location_of_birth
)
{
  return species(
    create_new_species_id(),
    parent.get_species_id(), //parent ID
    parent.get_clade_id(),
    time_of_birth,
    location_of_birth
  );
}

double elly::get_lowest_t_colonization(const species& s)
{
  const auto& v = s.get_times_of_colonization();
  assert(!v.empty());
  return *std::min_element(
    std::begin(v), std::end(v));
}

double elly::get_t_birth_mainland(const species& s) noexcept
{
  assert(s.get_location_of_birth() != location::both);

  if(s.get_location_of_birth() == location::island)
    return -1.0;
  if(s.get_location_of_birth() == location::mainland)
    return s.get_time_of_birth();
  assert(!"species can only be born on island or mainland"); //!OCLINT accepted idiom
  throw std::logic_error("species can only be born on island or mainland");
}

double elly::get_t_birth_island(const species& s) noexcept
{
  assert(s.get_location_of_birth() != location::both);

  if(s.get_location_of_birth() == location::island)
    return s.get_time_of_birth();
  if(s.get_location_of_birth() == location::mainland)
    return -1.0;
  assert(!"species can only be born on island or mainland"); //!OCLINT accepted idiom
  throw std::logic_error("species can only be born on island or mainland");
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
  if(s.get_times_of_colonization().empty())
  {
    return -1.0; //TODO should not return -1.0 eventually
  }
  return s.get_times_of_colonization().back();
}

const std::vector<double>& elly::species::get_times_of_colonization() const noexcept
{
  assert(std::count_if(
    std::begin(m_times_of_colonization),
    std::end(m_times_of_colonization),
    [](const double t)
    {
      return t < 0.0;
    }
  ) == 0);
  return m_times_of_colonization;
}

void elly::species::go_extinct(
  const double time_of_extinction,
  const location location_of_ext
)
{
  set_time_of_extinction(time_of_extinction, location_of_ext);
}

void elly::species::migrate_to_island(const double colonization_time)
{
  #ifndef NDEBUG
  const int n_migration_before =m_times_of_colonization.size();
  #endif

  if (is_on_island_only(*this))
  {
    throw std::logic_error("An island-only species cannot migrate to the island");
  }
  add_time_of_colonisation(colonization_time);

  if(m_time_of_extinction_is > 0.0)
    m_time_of_extinction_is = -1.0;


  #ifndef NDEBUG
  const int n_migration_after = m_times_of_colonization.size();
  assert(n_migration_after > n_migration_before);
  #endif
}

void elly::species::add_time_of_colonisation(const double time_of_colonization)
{
  if (!get_times_of_colonization().empty())
  {
    #ifdef WARN_ON_COLONIZATION
    std::clog << "Warning: recolonization of species #" << m_species_id
      << " at time " << time_of_colonization
      << ", keeping newer time of "
      << time_of_colonization << '\n'
    ;
    #endif // WARN_ON_COLONIZATION
    m_times_of_colonization.push_back(time_of_colonization);
    return;
  }
  assert(get_times_of_colonization().empty());
  if (time_of_colonization < 0.0)
  {
    throw std::logic_error("time of colonization must be positive");
  }
  m_times_of_colonization.push_back(time_of_colonization);

}

void elly::species::set_time_of_extinction(const double time_of_extinction, const location place)
{
  if (time_of_extinction < 0.0)
  {
    throw std::logic_error("time of extinction must be positive");
  }
  if(place == location::mainland)
  {
    m_time_of_extinction_main = time_of_extinction;
    return;
  }
  if(place == location::island)
  {
    m_time_of_extinction_is = time_of_extinction;
    return;
  }
  assert(!"Cannot go extinct at locations both, mainland_only, nor island_only"); //!OCLINT accepted idiom
}

bool elly::is_colonist(const species& s) noexcept
{
  return !s.get_times_of_colonization().empty();
}

bool elly::is_extant(const species& s) noexcept
{
  assert(s.get_location_of_birth() == location::mainland ||
         s.get_location_of_birth() == location::island);
  return is_on_island(s) || is_on_mainland(s);
}

bool elly::is_extinct(const species& s) noexcept
{
  return !is_extant(s);
}

bool elly::is_islander(const species& s) noexcept
{
  return is_colonist(s)
    || s.get_location_of_birth() == location::island
  ;
}

bool elly::is_mainlander(const species& s) noexcept
{
  return s.get_location_of_birth() == location::mainland;
}

bool elly::is_on(const species& s, const location any_location) noexcept
{
  switch (any_location)
  {
    case location::both: return is_on_both(s);
    case location::island: return is_on_island(s);
    case location::island_only: return is_on_island_only(s);
    case location::mainland: return is_on_mainland(s);
    case location::mainland_only: return is_on_mainland_only(s);
  }
  throw std::logic_error("location not implemented");
}

bool elly::is_on_both(const species& s) noexcept
{
  return is_on_island(s) && is_on_mainland(s);
}

bool elly::is_on_island(const species& s) noexcept
{
  //Colonization from mainland that still lives
  if(s.get_location_of_birth() == location::mainland)
  {
    return !s.get_times_of_colonization().empty()
      && s.get_time_of_extinction_island() == -1.0
    ;
  }
  //Island born species that still lives
  assert(s.get_location_of_birth() == location::island);
  //No migration to mainland
  return s.get_time_of_extinction_island() == -1.0;
}
bool elly::is_on_island_only(const species& s) noexcept
{
  return is_on_island(s) && !is_on_mainland(s);
}

bool elly::is_on_mainland(const species& s) noexcept
{
  //assuming no migration to mainland
  return s.get_location_of_birth() == location::mainland
    && s.get_time_of_extinction_mainland() == -1.0
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

bool elly::colonisation_times_are_equal(const std::vector<double>& lhs,
                                        const std::vector<double>& rhs) noexcept
{
  if (lhs.size() != rhs.size())
    {
      return false;
    }
  for(int i = 0; i < static_cast<int>(lhs.size()); ++i)
  {
    if(lhs[i] != rhs[i])
    return false;
  }
  return true;
}

void elly::species::replace_last_time_of_colonisation(const double t)
{
  assert(!get_times_of_colonization().empty());
  m_times_of_colonization.back() = t;
}

bool elly::operator==(const species& lhs, const species& rhs) noexcept
{
  return
       lhs.get_clade_id() == rhs.get_clade_id()
    && lhs.get_location_of_birth() == rhs.get_location_of_birth()
    && lhs.get_parent_id() == rhs.get_parent_id()
    && lhs.get_species_id() == rhs.get_species_id()
    && lhs.get_time_of_birth() == rhs.get_time_of_birth()
    && colonisation_times_are_equal(lhs.get_times_of_colonization(),
                                    rhs.get_times_of_colonization())
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

std::ostream& elly::operator<<(std::ostream& os, const species& s) noexcept
{
  os
    << "CID: " << s.get_clade_id() << ' '
    << ", ID: " << s.get_species_id() << ' '
    << ", PID: " << s.get_parent_id() << ' '
    << ", t_birth: " << s.get_time_of_birth() << ' '
    << "@ " << s.get_location_of_birth() << ' '
    << ", t_col ";
       std::copy(
         std::begin(s.get_times_of_colonization()),
         std::end(s.get_times_of_colonization()),
         std::ostream_iterator<double>(os, ",")
       );
    os
    << ' '
    << ", t_ext_main: " << s.get_time_of_extinction_mainland() << ' '
    << ", t_ext_is: " << s.get_time_of_extinction_island()
  ;
  return os;
}
