#include "distancer_helper.h"
#include <cassert>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "is_regular_file.h"

int count_different_bits(
  const boost::dynamic_bitset<>& a,
  const boost::dynamic_bitset<>& b
)
{
  if (a.size() != b.size())
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "size differs (" << a.size() << " versus " << b.size() << ")";
    throw std::invalid_argument(msg.str());
  }
  return (a ^ b).count();
}

boost::dynamic_bitset<> create_offspring(
  const boost::dynamic_bitset<>& p,
  const boost::dynamic_bitset<>& q,
  const boost::dynamic_bitset<>& inherit_from_p
)
{
  if (p.size() != q.size())
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "SIL size differs (" << p.size() << " versus " << q.size() << ")";
    throw std::invalid_argument(msg.str());
  }
  if (p.size() != inherit_from_p.size())
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "SIL size differs (" << p.size() << ") from inheritance bits size (" << inherit_from_p.size() << ")";
    throw std::invalid_argument(msg.str());
  }
  return (inherit_from_p & p) | (~inherit_from_p & q);
}

dna create_offspring(
  const dna& p,
  const dna& q,
  const boost::dynamic_bitset<>& inherit_from_p
)
{
  if (p.size() != q.size())
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "DNA size differs (" << p.size() << " versus " << q.size() << ")";
    throw std::invalid_argument(msg.str());
  }
  if (p.size() != inherit_from_p.size())
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "DNA size differs (" << p.size() << ") from inheritance bits size (" << inherit_from_p.size() << ")";
    throw std::invalid_argument(msg.str());
  }

  const auto sz = p.size();
  dna r{q};
  for (size_t i = 0; i!=sz; ++i)
  {
    if ( (1 << (sz - 1 - i)) //Use little-endian
      & inherit_from_p.to_ulong() )
    {
      r[i] = p[i];
    }
  }
  return r;
}

std::vector<int> create_tally(const std::vector<int>& v) noexcept
{
  std::map<int, int> m;
  for (const auto i: v)
  {
    const auto iter = m.find(i);
    if (iter == std::end(m))
    {
      m.insert(std::make_pair(i, 1));
    }
    else { ++m[i]; }
  }

  std::vector<int> t;
  t.reserve(m.size());
  for (const auto p: m) {
    t.push_back(p.second);
  }
  return t;
}

