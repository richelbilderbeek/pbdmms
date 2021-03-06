#include "ribi_individual.h"

#include <sstream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/math/special_functions/factorials.hpp>
#include "ribi_helper.h"
#include "ribi_create_tally.h"

ribi::individual::individual(
  const pin_t& any_pin,
  const sil_t& any_sil
) : m_pin{any_pin},
    m_sil{any_sil}
{

}

bool ribi::can_mate(
  const individual& a,
  const individual& b,
  const int max_genetic_distance
)
{
  //Is only determined by the SILs (Species Identity Loci)
  return can_mate(
    a.get_sil(),
    b.get_sil(),
    max_genetic_distance
  );
}

std::vector<int> ribi::count_abundances(
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
  ///ids will be {0,0,1,1,1}
  const auto m = create_tally(ids);
  ///m will be {{0,2},{1,3}}
  assert(m.find(-1) == std::end(m)); //Cannot be negative component indices
  assert((*m.find(0)).second >= 1); //Component index of 0 is always present at least once
  std::vector<int> v;
  v.reserve(m.size());
  for (const auto& i: m) { v.push_back(i.second); }
  return v;
}

int ribi::count_possible_species(std::vector<individual> p, const int max_genetic_distance) noexcept
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

int ribi::count_species(std::vector<individual> p, const int max_genetic_distance) noexcept
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

ribi::individual ribi::create_offspring(
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

int ribi::get_genetic_distance(
  const individual& a,
  const individual& b
) noexcept
{
  return get_genetic_distance(a.get_sil(), b.get_sil());
}

void ribi::mutate(
  individual& i,
  const probability pin_mutation_rate,
  const probability sil_mutation_rate,
  std::mt19937& rng_engine
)
{
  mutate_pins(i, pin_mutation_rate, rng_engine);
  mutate_sils(i, sil_mutation_rate, rng_engine);
}

void ribi::mutate_pins(
  individual& i,
  const probability pin_mutation_rate,
  std::mt19937& rng_engine
)
{
  const int n_loci{static_cast<int>(i.get_pin().size())};
  assert(n_loci);

  //How many loci will mutate?
  const double n_expected_mutations{
    pin_mutation_rate.get() * static_cast<double>(n_loci)
  };
  //n_muts_dist: number of mutations distribution
  std::poisson_distribution<int> n_muts_dist(n_expected_mutations);
  const int n_mutations{n_muts_dist(rng_engine)};
  //Which loci will mutate?
  const std::vector<std::size_t> v{
    get_unique_indices(
      i.get_pin().size(),
      n_mutations,
      rng_engine
    )
  };
  //Mutate those loci
  for (const std::size_t index: v)
  {
    i.get_pin().change(index, rng_engine);
  }
}

void ribi::mutate_sils(
  individual& i,
  const probability sil_mutation_rate,
  std::mt19937& rng_engine
)
{
  const int n_loci{static_cast<int>(i.get_sil().size())};
  assert(n_loci);

  //How many loci will mutate?
  const double n_expected_mutations{
    sil_mutation_rate.get() * static_cast<double>(n_loci)
  };
  //m_muts_dits = number of mutations distribution
  std::poisson_distribution<int> n_muts_dist(n_expected_mutations);
  const int n_mutations{n_muts_dist(rng_engine)};
  //Which loci will mutate?
  const std::vector<std::size_t> v{
    get_unique_indices(
      i.get_sil().size(),
      n_mutations,
      rng_engine
    )
  };
  //Mutate those loci
  for (const std::size_t index: v)
  {
    i.get_sil().flip(index);
  }
}


bool ribi::operator==(const individual& lhs, const individual& rhs) noexcept
{
  return
       lhs.get_pin() == rhs.get_pin()
    && lhs.get_sil() == rhs.get_sil()
  ;
}

bool ribi::operator!=(const individual& lhs, const individual& rhs) noexcept
{
  return !(lhs == rhs);
}

std::ostream& ribi::operator<<(std::ostream& os, const individual& i) noexcept
{
  os << i.get_sil();
  return os;
}
