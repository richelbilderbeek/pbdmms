#include "elly_clade.h"

#include <algorithm>
#include <cassert>
#include <vector>
#include <iterator>

#include "elly_species.h"

elly::clade::clade(
  const std::vector<species>& clade_species
) : m_clade_species{clade_species}
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
  const species first = s[0];
  return std::all_of(s.begin(), s.end(),
    [first] (const species t)
    { return t.get_clade_id() == first.get_clade_id(); });
}

elly::clade_id elly::clade::get_id() const noexcept
{
  assert(!m_clade_species.empty());
  return m_clade_species.back().get_clade_id();
}

elly::clade elly::get_islanders(const clade& c)
{
  const auto& all_species = c.get_species();
  std::vector<species> v;
  std::copy_if(
    std::begin(all_species),
    std::end(all_species),
    std::back_inserter(v),
    [](const species& s)
    {
      return is_islander(s);
    }
  );
  return v;
}

elly::clade elly::overestimate_colonization_time(const clade& c)
{
  std::clog << __func__ << ":STUB TODO\n";
  return c;
}
