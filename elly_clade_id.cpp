#include "elly_clade_id.h"

#include <cassert>

elly::clade_id::clade_id(const int id)
  : m_id{id}
{

}

elly::clade_id elly::create_new_clade_id() noexcept
{
  static int sm_next_id{1};
  assert(sm_next_id != 0); //This is the null clade ID
  return clade_id(sm_next_id++);
}

elly::clade_id elly::create_null_clade_id() noexcept
{
  return clade_id(0);
}

