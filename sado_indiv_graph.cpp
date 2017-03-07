#include "sado_indiv_graph.h"

#include <boost/graph/adjacency_list.hpp>
#include "has_edge_between_vertices.h"
#include "sado_individual.h"
#include "sado_parameters.h"
#include "sado_individual_vertex.h"
#include "sado_int_edge.h"

sado::pedigree_graph
sado::create_test_predigree_graph_1() noexcept
{
  pedigree_graph g{};
  const parameters p{create_golden_standard_parameters()};

  const indiv mother;
  const indiv father;
  const indiv kid = create_offspring(mother, father, p);

  const auto vd_a = add_individual_vertex(mother, g);
  const auto vd_b = add_individual_vertex(father, g);
  const auto vd_c = add_individual_vertex(kid, g);
  boost::add_edge(vd_a, vd_c, g);
  boost::add_edge(vd_b, vd_c, g);
  return g;
}

sado::pedigree_graph sado::create_graph_from_population(const std::vector<indiv>& pop) noexcept
{
  pedigree_graph g{};

  using vertex_des = typename boost::graph_traits<sado::pedigree_graph>::vertex_descriptor;

  std::vector<std::pair<int, vertex_des>> v;
  for(int i = 0; i != static_cast<int>(pop.size()); ++i)
  {
    const auto vd = add_individual_vertex(pop[i], g);
    const std::pair<int, vertex_des>
        vd_pair{i, vd};
    v.push_back(vd_pair);
  }

  //Connect offspring with father or mother
  for (const std::pair<int, vertex_des> pair : v)
  {
    for(int j = 0; j != static_cast<int>(pop.size()); ++j)
    {
      if (pop[pair.first].get_father_id() == pop[j].get_id()
          || pop[pair.first].get_mother_id() == pop[j].get_id())
       sado::add_int_edge(v[j].second, pair.second, 1, g);
    }
  }

  return g;
}
