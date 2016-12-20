#include "elly_species_id.h"

#include <cassert>

elly::species_id::species_id(const int id)
  : m_id{id}
{

}

elly::species_id elly::create_new_species_id() noexcept
{
  static int sm_next_id{1};
  assert(sm_next_id != 0); //This is the null species ID
  return species_id(sm_next_id++);
}

elly::species_id elly::create_null_species_id() noexcept
{
  return species_id(0);
}

