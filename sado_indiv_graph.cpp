#include "sado_indiv_graph.h"

#include <boost/graph/adjacency_list.hpp>
#include "has_edge_between_vertices.h"
#include "sado_individual.h"
#include "sado_parameters.h"
#include "sado_individual_vertex.h"
#include "sado_int_edge.h"

sado::pedigree_graph sado::create_graph_from_population(const std::vector<indiv>& pop) noexcept
{
  pedigree_graph g{};

  std::vector<std::pair<int, vert_desc>> v;
  for(int i = 0; i != static_cast<int>(pop.size()); ++i)
  {
    const auto vd = add_individual_vertex(pop[i], g);
    const std::pair<int, vert_desc>
        vd_pair{i, vd};
    v.push_back(vd_pair);
  }

  //Connect offspring with father or mother
  for (const std::pair<int, vert_desc> pair : v)
  {
    for(int j = 0; j != static_cast<int>(pop.size()); ++j)
    {
      if (pop[pair.first].get_father_id() == pop[j].get_id()
          || pop[pair.first].get_mother_id() == pop[j].get_id())
       add_int_edge(v[j].second, pair.second, 1, g);
    }
  }
  return g;
}
