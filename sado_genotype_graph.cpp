#include "sado_genotype_graph.h"

#include "sado_helper.h"
#include "sado_parameters.h"
#include "sado_attractiveness_matrix.h"
#include "count_undirected_graph_connected_components.h"

void sado::add_vertices(
  const population& pop,
  individual_graph& g
)
{
  const int n{pop.size()};
  for (int i{0}; i!=n; ++i)
  {
    boost::add_vertex(pop[i], g);
  }
}

void sado::add_edges(
  const attractiveness_matrix& as,
  individual_graph& g,
  const double min_attractiveness
)
{
  return add_edges_impl_2(as, g, min_attractiveness);
}

void sado::add_edges_impl_1(
  const attractiveness_matrix& as,
  individual_graph& g,
  const double min_attractiveness
)
{
  assert(is_valid(as));
  const int n_rows{static_cast<int>(as.size())};
  const int n_cols{n_rows};

  for (int i=0; i!=n_rows; ++i)
  {
    for (int j=0; j!=n_cols; ++j)
    {
      assert(i >= 0);
      assert(i < static_cast<int>(as.size()));
      assert(j >= 0);
      assert(j < static_cast<int>(as[i].size()));
      const double p{as[i][j]};
      if (p > min_attractiveness)
      {
        const auto vip = vertices(g);
        auto from_iter = vip.first + i;
        auto to_iter = vip.first + j;
        boost::add_edge(*from_iter, *to_iter, g);
      }
    }
  }
}

void sado::add_edges_impl_2( //!OCLINT indeed a complex function!
  const attractiveness_matrix& as,
  individual_graph& g,
  const double min_attractiveness
)
{
  assert(is_valid(as));
  const int n_rows{static_cast<int>(as.size())};
  const int n_cols{n_rows};

  std::vector<int> colors(n_cols, 0); //color
  int next_color{1};

  for (int i=0; i!=n_rows; ++i)
  {
    if (colors[i] == 0)
    {
      colors[i] = next_color;
      ++next_color;
    }
    for (int j=0; j!=n_cols; ++j)
    {
      assert(i >= 0);
      assert(i < static_cast<int>(as.size()));
      assert(j >= 0);
      assert(j < static_cast<int>(as[i].size()));
      if (colors[i] == colors[j]) continue;
      const double p{as[i][j]};
      if (p > min_attractiveness)
      {
        const auto vip = vertices(g);
        auto from_iter = vip.first + i;
        auto to_iter = vip.first + j;
        boost::add_edge(*from_iter, *to_iter, g);
        if (colors[j] == 0)
        {
          colors[j] = colors[i];
        }
      }
    }
  }
}

void sado::add_edges(
  const attractiveness_matrix& as,
  individual_graph& g,
  const parameters& p
)
{
  return add_edges(as, g, p.get_at());
}

sado::individual_graph sado::create_genotype_graph(
  const population& pop,
  const parameters& p)
{
  const attractiveness_matrix as{
    create_attractiveness_matrix(pop, p)
  };
  assert(is_valid(as));
  individual_graph g;
  add_vertices(pop, g);
  add_edges(as, g, p);
  return g;
}
