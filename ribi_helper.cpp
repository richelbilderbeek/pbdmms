#include "ribi_helper.h"
#include <cassert>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "is_regular_file.h"
#include "gsl/gsl_assert"
std::vector<std::string> ribi::convert_arguments(
  const int argc, const char * const argv[]) noexcept
{
   Expects(argc >= 0); //!OCLINT GSL gives double negative
   std::vector<std::string> v;
   v.reserve(argc);
   for (int i=0; i!=argc; ++i)
   {
     v.push_back(std::string(argv[i]));
   }
   Ensures(argc == static_cast<int>(v.size())); //!OCLINT GSL gives double negative
   return v;
}

int ribi::count_different_bits(
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

boost::dynamic_bitset<> ribi::create_offspring(
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
      << "SIL size differs (" << p.size()
      << ") from inheritance bits size ("
      << inherit_from_p.size() << ")"
    ;
    throw std::invalid_argument(msg.str());
  }
  return (inherit_from_p & p) | (~inherit_from_p & q);
}

ribi::dna ribi::create_offspring(
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
      << "DNA size differs (" << p.size()
      << ") from inheritance bits size ("
      << inherit_from_p.size() << ")"
    ;
    throw std::invalid_argument(msg.str());
  }

  const auto sz = p.size();
  dna r{q};
  for (size_t i = 0; i!=sz; ++i)
  {
    const long unsigned int bit_result{
        (1 << (sz - 1 - i)) //Use little-endian
      & inherit_from_p.to_ulong()
    };
    if (bit_result)
    {
      r[i] = p[i];
    }
  }
  return r;
}

std::vector<std::size_t> ribi::get_unique_indices(
  const std::size_t sz,
  const int n,
  std::mt19937& rng_engine
)
{
  assert(n >= 0);
  if (n == 0) return {};
  if (n > static_cast<int>(sz))
  {
    std::vector<std::size_t> v(sz);
    std::iota(std::begin(v), std::end(v), 0);
    return v;
  }
  std::set<std::size_t> s;
  std::uniform_int_distribution<std::size_t> d(0, n - 1);
  while (n != static_cast<int>(s.size()))
  {
    s.insert(d(rng_engine));
  }
  std::vector<std::size_t> v;
  std::copy(std::begin(s), std::end(s), std::back_inserter(v));
  return v;
}
