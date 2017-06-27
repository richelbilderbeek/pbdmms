#ifndef RIBI_ANCESTRY_GRAPH_H
#define RIBI_ANCESTRY_GRAPH_H

#include <boost/graph/adjacency_list.hpp>

#include "ribi_species.h"

namespace ribi {

using ancestry_graph = boost::adjacency_list<
  boost::vecS,
  boost::vecS,
  boost::bidirectionalS, // Both in_edges and out_edges exist
  //boost::directedS, //Only out_edges known
  species
>;

} //~namespace ribi

#endif // RIBI_ANCESTRY_GRAPH_H
