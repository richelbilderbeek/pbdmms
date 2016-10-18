#include "ribi_sil.h"
#include <boost/graph/adjacency_list.hpp>
#include "ribi_helper.h"
#include "count_max_number_of_pieces.h"

int ribi::count_possible_species(std::vector<sil> p, const int max_genetic_distance) noexcept
{
  if (p.empty()) return 0;

  //Ditch the duplicates, don't care about order
  sort_and_remove_duplicates(p);

  const int sz{static_cast<int>(p.size())};
  if (sz == 1) return 1;

  //Connect all species
  boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> g(sz);
  for (int i=0; i!=sz; ++i)
  {
    for (int j=i+1; j!=sz; ++j)
    {
      const int genetic_distance{get_genetic_distance(p[i],p[j])};
      if (genetic_distance <= max_genetic_distance)
      {
        const auto vip = vertices(g);
        auto from_iter = vip.first + i;
        auto to_iter = vip.first + j;
        boost::add_edge(*from_iter, *to_iter, g);
      }
    }
  }

  return count_max_number_of_pieces(g);
}

ribi::sil ribi::create_sil(const std::string& s)
{
  const auto sz = s.size();
  sil r(sz, 0);
  for (std::size_t i = 0; i!=sz; ++i)
  {
    const char c = s[i];
    if (c != '0' && c != '1')
    {
      throw std::invalid_argument("SIL must be zeroes and ones");
    }
    if (c == '1')
    {
      r[i] = true;
    }
  }
  return r;
}

int ribi::get_genetic_distance(
  const sil& a,
  const sil& b
) noexcept
{
  assert(a.size() == b.size());
  const sil d = a ^ b;
  return d.count();
}

void ribi::sort_and_remove_duplicates(std::vector<sil>& p) noexcept
{
  std::sort(
    std::begin(p),std::end(p), [](const sil& lhs, const sil& rhs)
    {
      return lhs.to_ulong() < rhs.to_ulong();
    }
  );
  typename std::vector<sil>::iterator new_end = std::unique(std::begin(p),std::end(p));
  p.erase(new_end,std::end(p));
}

