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

///Obtain a testing phylogeny
/*
           2--4
          /    \
 Past 0--1      6--7 Present
          \    /
           3--5
*/
sil_frequency_phylogeny get_test_sil_frequency_phylogeny_2() noexcept;


///Move all SIL connection from 'from' to 'to'
/*
           2
          / \
 Past 0--1   4--5 Present
          \ /
           3


becomes

           2
         // \\
 Past 0--1   4--5 Present

           3

2: to
3: froms[0]
*/
void move_sil_connection(
  const sil_frequency_vertex_descriptor from,
  const sil_frequency_vertex_descriptor to,
  sil_frequency_phylogeny& g
);

///Used by move_sil_connection
void move_sil_connection_pre(
  const sil_frequency_vertex_descriptor from,
  const sil_frequency_vertex_descriptor to,
  sil_frequency_phylogeny& g
);

///Used by move_sil_connection
void move_sil_connection_post(
  const sil_frequency_vertex_descriptor from,
  const sil_frequency_vertex_descriptor to,
  sil_frequency_phylogeny& g
);

///Move all SIL connection from 'froms' to 'to'
/*
           2
          / \
 Past 0--1   4--5 Present
          \ /
           3


becomes

           2
         // \\
 Past 0--1   4--5 Present

           3

2: to
3: froms[0]
*/
void move_sil_connections(
  const sil_frequency_vertex_descriptors& froms,
  const sil_frequency_vertex_descriptor to,
  sil_frequency_phylogeny& g
);


///Move all SIL frequences from 'froms' to 'to'
///Clears all 'froms'
void move_sil_frequencies(
  const sil_frequency_vertex_descriptors& froms,
  const sil_frequency_vertex_descriptor to,
  sil_frequency_phylogeny& g
);

std::ostream& operator<<(std::ostream& os, const sil_frequency_phylogeny& r) noexcept;

} //~namespace ribi

#endif // DISTANCER_RESULTS_GRAPH_H
