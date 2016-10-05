#include "distancer_individual.h"

#include <sstream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/math/special_functions/factorials.hpp>
#include "distancer_helper.h"

individual::individual(
  const pin_t& any_pin,
  const sil_t& any_sil
) : m_pin{any_pin},
    m_sil{any_sil}
{

}

std::vector<int> count_abundances(
  std::vector<individual> p,
  const int max_genetic_distance
) noexcept
{
  if (p.empty()) return {};

  const int sz{static_cast<int>(p.size())};
  if (sz == 1) return { static_cast<int>(p.size()) };

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
  const auto ids = get_connected_components_ids(g);
  return create_tally(ids);
}

int count_possible_species(std::vector<individual> p, const int max_genetic_distance) noexcept
{
  if (p.empty()) return 0;
  std::vector<sil> sils;
  sils.reserve(p.size());
  std::transform(std::begin(p), std::end(p),
    std::back_inserter(sils),
    [](const individual& i) { return i.get_sil(); }
  );
  return count_possible_species(sils, max_genetic_distance);
}

int count_species(std::vector<individual> p, const int max_genetic_distance) noexcept
{
  //const bool debug{false};
  if (p.empty()) return 0;

  //Ditch the duplicates to speed up the calculation
  std::sort(
    std::begin(p),std::end(p), [](const individual& lhs, const individual& rhs)
    {
      return lhs.get_sil().to_ulong() < rhs.get_sil().to_ulong();
    }
  );
  typename std::vector<individual>::iterator new_end = std::unique(std::begin(p),std::end(p));
  p.erase(new_end,std::end(p));

  const int sz{static_cast<int>(p.size())};
  if (sz == 1) return 1;

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
  return count_connected_components(g);
}

individual create_offspring(
  const individual& p,
  const individual& q,
  const boost::dynamic_bitset<>& inherit_pin_from_p,
  const boost::dynamic_bitset<>& inherit_sil_from_p
)
{
  if (p.get_pin().size() != q.get_pin().size())
  {
    std::stringstream msg;
    msg << __func__ << ": PIN lengths differ: '"
      << p.get_pin() << "' and '"
      << q.get_pin() << "''"
    ;
    throw std::invalid_argument(msg.str());
  }
  if (p.get_pin().size() != inherit_pin_from_p.size())
  {
    std::stringstream msg;
    msg << __func__ << ": PIN and inheritance lengths differ";
    throw std::invalid_argument(msg.str());
  }
  if (p.get_sil().size() != q.get_sil().size())
  {
    std::stringstream msg;
    msg << __func__ << ": SIL lengths differ: '"
      << p.get_sil() << "' and '"
      << q.get_sil() << "''"
    ;
    throw std::invalid_argument(msg.str());
  }
  if (p.get_sil().size() != inherit_sil_from_p.size())
  {
    std::stringstream msg;
    msg << __func__ << ": SIL and inheritance lengths differ";
    throw std::invalid_argument(msg.str());
  }
  const auto pin = create_offspring(p.get_pin(), q.get_pin(), inherit_pin_from_p);
  const auto sil = create_offspring(p.get_sil(), q.get_sil(), inherit_sil_from_p);
  return individual(pin, sil);
}

int get_genetic_distance(
  const individual& a,
  const individual& b
) noexcept
{
  return get_genetic_distance(a.get_sil(), b.get_sil());
}


bool operator==(const individual& lhs, const individual& rhs) noexcept
{
  return
       lhs.get_pin() == rhs.get_pin()
    && lhs.get_sil() == rhs.get_sil()
  ;
}

bool operator!=(const individual& lhs, const individual& rhs) noexcept
{
  return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, const individual& i) noexcept
{
  os << i.get_sil();
  return os;
}
