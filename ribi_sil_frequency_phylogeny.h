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

using sil_frequency_vertex_descriptors
  = std::vector<sil_frequency_vertex_descriptor>;

using sil_frequency_vertex_descriptor_pair
  = std::pair<sil_frequency_vertex_descriptor,sil_frequency_vertex_descriptor>;

using sil_frequency_vertex_descriptor_pairs
  = std::vector<sil_frequency_vertex_descriptor_pair>;

///Add a sil_frequency_edge between two vertices
void add_sil_frequency_edge(
  const sil_frequency_edge edge,
  const sil_frequency_vertex_descriptor& from,
  const sil_frequency_vertex_descriptor& to,
  sil_frequency_phylogeny& g
);


///Obtain a testing phylogeny
/*
           2
          / \
 Past 0--1   4--5 Present
          \ /
           3
*/
sil_frequency_phylogeny get_test_sil_frequency_phylogeny_1() noexcept;

std::ostream& operator<<(std::ostream& os, const sil_frequency_phylogeny& r) noexcept;

} //~namespace ribi

#endif // DISTANCER_RESULTS_GRAPH_H
