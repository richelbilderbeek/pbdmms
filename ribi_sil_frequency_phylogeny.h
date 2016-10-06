#ifndef DISTANCER_RESULTS_GRAPH_H
#define DISTANCER_RESULTS_GRAPH_H

#include <iosfwd>
#include <boost/graph/adjacency_list.hpp>
#include "ribi_sil_frequency_edge.h"
#include "ribi_sil_frequency_vertex.h"

namespace ribi {

using sil_frequency_phylogeny = boost::adjacency_list<
  boost::vecS,
  boost::vecS,
  boost::undirectedS,
  sil_frequency_vertex,
  sil_frequency_edge
>;

using sil_frequency_vertex_descriptor
  = boost::graph_traits<sil_frequency_phylogeny>::vertex_descriptor;

std::ostream& operator<<(std::ostream& os, const sil_frequency_phylogeny& r) noexcept;

} //~namespace ribi

#endif // DISTANCER_RESULTS_GRAPH_H
