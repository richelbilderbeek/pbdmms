#ifndef SADO_MATE_GRAPH_H
#define SADO_MATE_GRAPH_H

#include <boost/graph/adjacency_list.hpp>

#include "sado_simulation.h"
#include "sado_fwd.h"
#include "sado_individual.h"
#include "sado_attractiveness_matrix.h"
namespace sado {

///A graph in which all individuals that can mate with p > 0.05 are
///connected
using mate_graph
  = boost::adjacency_list<
      boost::vecS, boost::vecS, boost::undirectedS, individual
    >;

///Add edges between genotypes for attractivenesses above a certain minimum
/// @param attractiveness attractiveness ordered that
///   v[observant][observed] equals the amount
///   the observant is attracted to the observed
void add_edges(
  const attractiveness_matrix& as,
  mate_graph& g,
  const parameters& p
);

///Add edges between genotypes for attractivenesses above a certain minimum
/// @param attractiveness attractiveness ordered that
///   v[observant][observed] equals the amount
///   the observant is attracted to the observed
/// @param min_attractiveness the minimum attractiveness to make a female willing to mate
void add_edges(
  const attractiveness_matrix& as,
  mate_graph& g,
  const double min_attractiveness
);

void add_edges_impl_1(
  const attractiveness_matrix& as,
  mate_graph& g,
  const double min_attractiveness
);

void add_edges_impl_2(
  const attractiveness_matrix& as,
  mate_graph& g,
  const double min_attractiveness
);

void add_vertices(
  const population& pop,
  mate_graph& g
);

///Creates a graph in which the individuals that can mate with p >= 0.05
///are connected
mate_graph create_mate_graph(
  const population& pop,
  const parameters& p);

/// Empty
mate_graph create_test_mate_graph1();

/// One individual
mate_graph create_test_mate_graph2();

/// Two individuals that can mate
mate_graph create_test_mate_graph3();

/// Two individuals that cannot mate
mate_graph create_test_mate_graph4();

///Copy all individuals in the mate graph, seperated
///per ID.
/// @param ids specied IDs, each value must be zero at least, for example,
///   as created by boost::connected_components
/// @param mate_graph a graph that connects individuals that can potentially mate
std::map<int, std::vector<individual>> seperate_individuals_by_id(
  std::vector<int> ids,
  const mate_graph& g
);

} //~namespace sado

#endif // SADO_MATE_GRAPH_H
