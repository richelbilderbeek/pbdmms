#ifndef SADO_SPECIES_GRAPH_H
#define SADO_SPECIES_GRAPH_H

#include <boost/graph/adjacency_list.hpp>
#include "sado_species.h"
#include <vector>

namespace sado {

using species_graph = boost::adjacency_list<
  boost::vecS,
  boost::vecS,
  boost::directedS,
  sado::sado_species,
  int
>;

using sp_vert_desc = boost::graph_traits<species_graph>::vertex_descriptor;

using edg_desc = boost::graph_traits<species_graph>::edge_descriptor;

species_graph
create_empty_directed_species_graph() noexcept;

species_graph
create_my_species_graph() noexcept;


species_graph create_graph_from_species_vector(const std::vector<sado::sado_species>& species) noexcept;

} //~namespace sado

#endif // SADO_SPECIES_GRAPH_H
