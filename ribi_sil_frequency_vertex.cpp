#include "ribi_sil_frequency_vertex.h"
#include <sstream>
#include <stdexcept>

int ribi::sil_frequency_vertex::s_m_next_id = 0; //!OCLINT must count number of instances

ribi::sil_frequency_vertex::sil_frequency_vertex()
  : m_id{s_m_next_id++},
    m_sil_frequencies{},
    m_style{sil_frequency_vertex_style::unknown},
    m_time{}
{

}


ribi::sil_frequency_vertex::sil_frequency_vertex(
  const std::map<sil,int>& sil_frequencies,
  const int time
)
  : m_id{s_m_next_id++},
    m_sil_frequencies{sil_frequencies},
    m_style{sil_frequency_vertex_style::unknown},
    m_time{time}
{

  for (const auto p: m_sil_frequencies)
  {
    if (p.second < 0)
    {
      std::stringstream msg;
      msg << __func__ << ": "
        << "frequency must be zero or positive, instead of "
        << p.second
      ;
      throw std::invalid_argument(msg.str());
    }
  }
  if (m_time < 0)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "time must be zero or positive, instead of "
      << m_time
    ;
    throw std::invalid_argument(msg.str());
  }
}

int ribi::count_n_possible_species(
  const sil_frequency_vertex& vertex,
  const int max_genetic_distance
) noexcept
{
  //Collect the sils
  const auto& sfs = vertex.get_sil_frequencies();
  std::vector<sil> sils;
  sils.reserve(sfs.size());
  std::transform(
    std::begin(sfs),
    std::end(sfs),
    std::back_inserter(sils),
    [](const std::pair<sil,int>& p)
    {
      return p.first;
    }
  );
  //Count the possible species
  return count_possible_species(sils, max_genetic_distance);
}

std::string ribi::get_sil_frequencies_str(const sil_frequency_vertex& v) noexcept
{

  const auto fs = v.get_sil_frequencies();
  std::stringstream s;
  std::transform(std::begin(fs), std::end(fs),
    std::ostream_iterator<std::string>(s, ","),
    [](const std::pair<sil,int>& p)
    {
      std::stringstream t;
      t << "(" << p.first << ", " << p.second << ")";
      return t.str();
    }
  );
  std::string t{s.str()};
  if (!t.empty()) t.pop_back();
  return t;
}

std::map<ribi::sil,int> ribi::merge_sil_frequencies(
  const std::map<sil,int>& lhs,
  const std::map<sil,int>& rhs
)
{
  auto fs = lhs;
  for (const auto& p: rhs)
  {
    const auto iter = fs.find(p.first);
    if (iter == std::end(fs))
    {
      //If {SIL,frequency} pair is absent, add it
      fs.insert(std::end(fs), p);
    }
    else
    {
      //If {SIL,frequency} pair is present, increase the frequency
      (*iter).second += p.second;
    }
  }
  return fs;
}

void ribi::move_sil_frequencies(sil_frequency_vertex& from, sil_frequency_vertex& to)
{
  if (from.get_time() != to.get_time())
  {
    throw std::invalid_argument("SIL frequencies may only be summed from the same generation");
  }
  to.m_sil_frequencies = merge_sil_frequencies(
    from.get_sil_frequencies(),
    to.get_sil_frequencies()
  );
  from.m_sil_frequencies.clear();
}

bool ribi::operator==(
  const sil_frequency_vertex& lhs,
  const sil_frequency_vertex& rhs
) noexcept
{
  return lhs.get_sil_frequencies() == rhs.get_sil_frequencies()
    && lhs.get_time() == rhs.get_time()
  ;
}

std::string ribi::to_str(const sil_frequency_vertex_style s) noexcept
{
  switch (s)
  {
    case sil_frequency_vertex_style::good: return "good";
    case sil_frequency_vertex_style::incipient: return "incipient";
    case sil_frequency_vertex_style::unknown: return "unknown";
  }
  assert(!"Should not get here"); //!OCLINT accepted idiom
  throw std::logic_error("Unimplemented sil_frequency_vertex_style");
}

std::ostream& ribi::operator<<(std::ostream& os, const sil_frequency_vertex_style& s) noexcept
{
  os << to_str(s);
  return os;
}

std::ostream& ribi::operator<<(std::ostream& os, const sil_frequency_vertex& v) noexcept
{
  os
    << "#" << v.get_id() << ", "
    << "t=" << v.get_time() << ", sfs=" << get_sil_frequencies_str(v);
  return os;
}
