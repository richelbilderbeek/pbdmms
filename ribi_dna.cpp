#include "ribi_dna.h"
#include <cassert>
#include <sstream>
#include <stdexcept>
#include "gsl/gsl_assert"

ribi::dna::dna(const std::string& sequence)
  : m_sequence{sequence}
{
  for (const auto& c: sequence)
  {
    if (c != 'A' && c != 'C' && c != 'G' && c != 'T')
    {
      std::stringstream msg;
      msg << __func__ << ": invalid nucleotide '"
        << c << "' supplied with sequences '"
        << sequence << "'";
      throw std::invalid_argument(msg.str());
    }
  }
}

void ribi::dna::change(const size_t i, std::mt19937& rng_engine) noexcept
{
  Expects(i < m_sequence.size()); //!OCLINT GSL gives double negative
  std::vector<char> v = { 'A','C','G','T' };
  for (std::size_t j{0}; j!=3; ++j)
  {
    if(v[j] == m_sequence[i]) { std::swap(v[j], v.back()); };
  }
  v.pop_back();
  std::uniform_int_distribution<size_t> nucleotide_index(0, 2); //[0,2]
  const size_t j = nucleotide_index(rng_engine);
  assert(j < v.size());
  m_sequence[i] = v[j];
}

const char& ribi::dna::operator[](const size_t i) const noexcept
{
  Expects(i < m_sequence.size()); //!OCLINT GSL gives double negative
  return m_sequence[i];
}

char& ribi::dna::operator[](const size_t i) noexcept
{
  Expects(i < m_sequence.size()); //!OCLINT GSL gives double negative
  return m_sequence[i];
}


std::ostream& ribi::operator<<(std::ostream& os, const dna& d) noexcept
{
  os << d.get_sequence();
  return os;
}

bool ribi::operator==(const dna& lhs, const dna& rhs) noexcept
{
  return lhs.get_sequence() == rhs.get_sequence();
}
