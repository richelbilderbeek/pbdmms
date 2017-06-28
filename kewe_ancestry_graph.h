#ifndef KEWE_ANCESTRY_GRAPH_H
#define KEWE_ANCESTRY_GRAPH_H

#include <boost/graph/adjacency_list.hpp>

#include "kewe_species.h"

namespace kewe {

using ancestry_graph = boost::adjacency_list<
  boost::vecS,
  boost::vecS,
  boost::bidirectionalS, // Both in_edges and out_edges exist
  //boost::directedS, //Only out_edges known
  species
>;

/*
   +-- generation since crown age
   |
   V

   |
 0 +   0
   |   |
 1 + +-1-+
   | |   |
 2 + 2   3 present

   Values are the node indices, where 0 is the species in time that is the ancestor.
   Lines are evolutionary relationships
*/
ancestry_graph create_test_graph_2() noexcept;

} //~namespace kewe

#endif // KEWE_ANCESTRY_GRAPH_H
