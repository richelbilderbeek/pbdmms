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

} //~namespace jobo

#endif // JOBO_ANCESTRY_GRAPH_H
