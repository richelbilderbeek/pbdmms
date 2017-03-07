#ifndef SADO_INDIV_GRAPH_H
#define SADO_INDIV_GRAPH_H

#include <boost/graph/adjacency_list.hpp>
#include "sado_individual.h"

namespace sado {

///Graph that resembled the heritage of individuals:
///each offspring is connected to its father and mother
using pedigree_graph = boost::adjacency_list<
  boost::vecS,
  boost::vecS,
  boost::directedS,
  sado::indiv,
  int
>;

using vert_desc = boost::graph_traits<pedigree_graph>::vertex_descriptor;

using indiv_edg_desc = boost::graph_traits<pedigree_graph>::edge_descriptor;

pedigree_graph create_test_predigree_graph_1() noexcept;

pedigree_graph create_graph_from_population(const std::vector<sado::indiv>& pop) noexcept;

} //~namespace sado



#endif // SADO_INDIV_GRAPH_H
