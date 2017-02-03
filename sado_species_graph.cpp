#include "sado_species_graph.h"

#include <boost/graph/adjacency_list.hpp>
#include "has_edge_between_vertices.h"
#include "sado_species.h"
#include "sado_parameters.h"
#include "sado_species_vertex.h"
#include "sado_int_edge.h"

sado::species_graph
create_empty_directed_species_graph() noexcept
{
  return {};
}

sado::species_graph
create_my_species_graph() noexcept
{

  auto g = create_empty_directed_species_graph();
  /*const sado::parameters p = sado::create_golden_standard_parameters();

  const sado::sado_species mother;
  const sado::sado_species father;
  const sado::sado_species kid = sado::create_offspring(mother, father, p);

  const auto vd_a = add_species_vertex(mother, g);
  const auto vd_b = add_species_vertex(father, g);
  const auto vd_c = add_species_vertex(kid, g);
  boost::add_edge(vd_a, vd_c, g);
  boost::add_edge(vd_b, vd_c, g);*/
  return g;

}

sado::species_graph create_graph_from_species_vector(const std::vector<sado::sado_species>& /* species */) noexcept
{
  auto g = create_empty_directed_species_graph();
/*
  using vertex_des = typename boost::graph_traits<sado::species_graph>::vertex_descriptor;

  std::vector<std::pair<int, vertex_des>> v;
  for(int i = 0; i != static_cast<int>(species.size()); ++i)
  {
    const auto vd = add_species_vertex(species[i], g);
    const std::pair<int, vertex_des>
        vd_pair{i, vd};
    v.push_back(vd_pair);
  }

  for (const std::pair<int, vertex_des> pair : v)
  {
    for(int j = 0; j != static_cast<int>(species.size()); ++j)
    {
      if (species[pair.first].get_father_id() == species[j].get_id()
          || species[pair.first].get_mother_id() == species[j].get_id())
       sado::add_int_edge(v[j].second, pair.second, 1, g);
    }
  }
*/
  return g;
}
