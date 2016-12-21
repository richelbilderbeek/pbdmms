#include "kewe_genotype_graph.h"

#include "kewe_helper.h"
#include "kewe_parameters.h"

void kewe::add_vertexes(
  const individuals& pop,
  genotype_graph& g
)
{
  for (int i = 0; i < static_cast<int>(pop.size()); ++i)
  {
    boost::add_vertex(std::to_string(i), g);
  }
}

void kewe::add_edges(
  const std::vector<std::vector<double>>& attractiveness,
  genotype_graph& g,
  const double minimal_attractiveness
)
{
  assert(!attractiveness.empty());
  assert(is_square(attractiveness));
  assert(has_diagonal_of_zeroes(attractiveness)); //Individuals do not find themselves attractive
  const int n_rows{static_cast<int>(attractiveness.size())};
  const int n_cols{n_rows};

  for (int i=0; i!=n_rows; ++i)
  {
    for (int j=0; j!=n_cols; ++j)
    {
      assert(i >= 0);
      assert(i < static_cast<int>(attractiveness.size()));
      assert(j >= 0);
      assert(j < static_cast<int>(attractiveness[i].size()));
      const double p{attractiveness[i][j]};
      if (p > minimal_attractiveness)
      {
        const auto vip = vertices(g);
        auto from_iter = vip.first + i;
        auto to_iter = vip.first + j;
        boost::add_edge(*from_iter, *to_iter, g);
      }
    }
  }
}

void kewe::add_edges(
  const std::vector<std::vector<double>>& attractiveness,
  genotype_graph& g,
  const simulation_parameters& parameters
)
{
  return add_edges(attractiveness, g, parameters.at);
}
