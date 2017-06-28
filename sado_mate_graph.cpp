#include "sado_mate_graph.h"

#include "sado_helper.h"
#include "sado_parameters.h"
#include "sado_attractiveness_matrix.h"
#include "count_undirected_graph_connected_components.h"

void sado::add_vertices(
  const population& pop,
  mate_graph& g
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
  mate_graph& g,
  const double min_attractiveness
)
{
  return add_edges_impl_2(as, g, min_attractiveness);
}

void sado::add_edges_impl_1(
  const attractiveness_matrix& as,
  mate_graph& g,
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
  mate_graph& g,
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
  mate_graph& g,
  const parameters& p
)
{
  return add_edges(as, g, p.get_at());
}

sado::mate_graph sado::create_mate_graph(
  const population& pop,
  const parameters& p)
{
  const attractiveness_matrix as{
    create_attractiveness_matrix(pop, p)
  };
  assert(is_valid(as));
  mate_graph g;
  add_vertices(pop, g);
  add_edges(as, g, p);
  return g;
}

sado::mate_graph sado::create_test_mate_graph1()
{
  return {};
}

sado::mate_graph sado::create_test_mate_graph2()
{
  mate_graph g;
  boost::add_vertex(individual(), g);
  return g;
}

sado::mate_graph sado::create_test_mate_graph3()
{
  mate_graph g;
  const auto vd1 = boost::add_vertex(individual(), g);
  const auto vd2 = boost::add_vertex(individual(), g);
  boost::add_edge(vd1, vd2, g);
  return g;
}

sado::mate_graph sado::create_test_mate_graph4()
{
  mate_graph g;
  boost::add_vertex(
    individual(phyg::create_null_id(), phyg::create_null_id(),
    -100.0, -100.0, -100.0, -100.0, -100.0, -100.0),
    g
  );
  boost::add_vertex(
    individual(phyg::create_null_id(), phyg::create_null_id(),
     100.0,  100.0,  100.0,  100.0,  100.0,  100.0),
    g
  );
  return g;
}

std::map<int, std::vector<sado::individual>> sado::seperate_individuals_by_id(
  std::vector<int> ids,
  const mate_graph& g
)
{
  if (ids.size() != boost::num_vertices(g))
  {
    throw std::invalid_argument("There must be as much IDs as individuals");
  }
  std::map<int, std::vector<individual>> m;
  //Give map all IDs, each one without species
  {
    for (const auto i: get_unique(ids)) m.insert( { i, {} } );
  }
  //Put all species in the map
  const auto vip = boost::vertices(g);
  int i{0};
  for (auto vi = vip.first; vi != vip.second; ++vi, ++i)
  {
    assert(i >= 0);
    assert(i < static_cast<int>(ids.size()));
    const int id{ids[i]};
    m[id].push_back(g[*vi]);
  }
  return m;
}
