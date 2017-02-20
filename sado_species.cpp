#include "sado_species.h"

sado::species::species(
    const int gen,
    const std::vector<indiv>& indivs)
  : m_id{create_new_id()}, m_generation{gen}, m_indivs{indivs}
{
}

bool sado::operator==(const species& lhs, const species& rhs) noexcept
{
  return lhs.get_indivs() == rhs.get_indivs() && lhs.get_id() == rhs.get_id();
}
bool sado::operator!=(const species& lhs, const species& rhs)  noexcept
{
  return !(lhs == rhs);
}

bool sado::species::empty() const noexcept
{
  return m_indivs.empty();
}

size_t sado::species::size() const noexcept
{
  return m_indivs.size();
}

const sado::indiv& sado::species::operator[](const int i) const
{
  return m_indivs[i];
}

