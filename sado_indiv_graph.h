#ifndef SADO_INDIV_GRAPH_H
#define SADO_INDIV_GRAPH_H

#include <boost/graph/adjacency_list.hpp>
#include "sado_individual.h"

namespace sado {

using indiv_graph = boost::adjacency_list<
  boost::vecS,
  boost::vecS,
  boost::directedS,
  sado::indiv,
  int
>;

using vert_desc = boost::graph_traits<indiv_graph>::vertex_descriptor;

using indiv_edg_desc = boost::graph_traits<indiv_graph>::edge_descriptor;

indiv_graph
create_empty_directed_individual_graph() noexcept;

indiv_graph
create_my_individual_graph() noexcept;



indiv_graph create_graph_from_population(const std::vector<sado::indiv>& pop) noexcept;

} //~namespace sado



#endif // SADO_INDIV_GRAPH_H
