#include "jobo_helper.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cassert>
#include <exception>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <boost/graph/adjacency_list.hpp>
#include "jobo_simulation.h"
#include "count_undirected_graph_connected_components.h"

void hello_jobo() noexcept
{
  std::cout <<"hello world" << '\n';
}

int add(int a, int b) noexcept
{
   return a + b;
}

double divide(double a, double b)
{
    if(b==0.0){
      throw std::invalid_argument("can't divide by 0");
    }
    return a/b;
}

int jobo::count_good_species(const std::vector<individual>& p)
{
  if (p.empty()) return 0;

  //Ditch the duplicates to speed up the calculation
  const std::vector<genotype> z = get_unique_genotypes(p);

  const int sz{static_cast<int>(z.size())};
  if (sz == 1) return 1;

  boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> g(sz);

  for (int i=0; i!=sz; ++i)
  {
    for (int j=i+1; j!=sz; ++j)
    {
      const double p{calc_chance_dead_kids(z[i], z[j])};
      if (p < 0.0001)
      {
        const auto vip = vertices(g);
        auto from_iter = vip.first + i;
        auto to_iter = vip.first + j;
        boost::add_edge(*from_iter, *to_iter, g);
      }
    }
  }
  return count_undirected_graph_connected_components(g);

}

int count_good_species(std::vector<int> chances)
{
int sz = 6;
int n_good_species = 1;
const int y{static_cast<int>(chances.size())};
for (int i=0; i!=sz-1; i+=1)
{
  if(chances[i]!=0) ++n_good_species;
}

if(n_good_species == sz)
{
  for (int i=y-1; i>(sz-2); i--)
  {
    if(chances[i]!=2) --n_good_species;
    if (n_good_species == 2)
    {
      break;
    }
  }
}
return n_good_species;
}
