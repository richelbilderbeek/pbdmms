#ifndef KEWE_GENOTYPE_GRAPH_H
#define KEWE_GENOTYPE_GRAPH_H

#include <boost/graph/adjacency_list.hpp>

#include "kewe_attractivenesses.h"
#include "kewe_fwd.h"
#include "kewe_individuals.h"

namespace kewe {

using genotype_graph
  = boost::adjacency_list<
      boost::vecS, boost::vecS, boost::undirectedS, std::string
    >;

///Add edges between genotypes for attractivenesses above a certain minimum
/// @param attractiveness attractiveness ordered that v[observant][observed] equals the amount
///   the observant is attracted to the observed
void add_edges(
  const attractivenesses& as,
  genotype_graph& g,
  const simulation_parameters& parameters
);

///Add edges between genotypes for attractivenesses above a certain minimum
/// @param attractiveness attractiveness ordered that v[observant][observed] equals the amount
///   the observant is attracted to the observed
/// @param min_attractiveness the minimum attractiveness to make a female willing to mate
void add_edges(
  const attractivenesses& as,
  genotype_graph& g,
  const double min_attractiveness
);

void add_edges_impl_1(
  const attractivenesses& as,
  genotype_graph& g,
  const double min_attractiveness
);

void add_edges_impl_2(
  const attractivenesses& as,
  genotype_graph& g,
  const double min_attractiveness
);

void add_vertices(
  const individuals& pop,
  genotype_graph& g
);


} //~namespace kewe

#endif // KEWE_GENOTYPE_GRAPH_H
