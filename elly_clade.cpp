#include "elly_clade.h"

#include <cassert>
#include <vector>
#include <iterator>

#include "elly_species.h"

elly::clade::clade(
  const clade_id id,
  const std::vector<species>& clade_species
) : m_id{id}, m_clade_species{clade_species}
{
  if (m_clade_species.empty())
  {
    throw std::invalid_argument("Clade must have at least one species");
  }
  if (!all_have_same_clade_id(m_clade_species))
  {
    throw std::invalid_argument("Species must all have the same clade ID");
  }
}

bool elly::all_have_same_clade_id(const std::vector<species>& s)
{
  assert(!s.empty());
  species first = s[0];

  return std::all_of(s.begin(), s.end(),
    [] (const species t)
    { return t.get_clade_id() == first.get_clade_id(); });
  /*
  for(const species next: s)
  {
    if(first.get_clade_id() != next.get_clade_id())
    {
      return false;
    }
  }
  return true;
  */
}

elly::clade_id elly::clade::get_id() const noexcept
{
  assert(!m_clade_species.empty());
  return m_clade_species.back().get_clade_id();
}
