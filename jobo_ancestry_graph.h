#ifndef JOBO_ANCESTRY_GRAPH_H
#define JOBO_ANCESTRY_GRAPH_H

#include <boost/graph/adjacency_list.hpp>

#include "jobo_species.h"

namespace jobo {

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

} //~namespace jobo

#endif // JOBO_ANCESTRY_GRAPH_H
