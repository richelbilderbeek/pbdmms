#ifndef KEWE_GENOTYPE_GRAPH_H
#define KEWE_GENOTYPE_GRAPH_H

#include <boost/graph/adjacency_list.hpp>

#include "kewe_fwd.h"
#include "kewe_individuals.h"

namespace kewe {

using genotype_graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, std::string>;

///Add edges between genotypes for attractivenesses above a certain minimum
/// @param attractiveness attractiveness ordered that v[observant][observed] equals the amount
///   the observant is attracted to the observed
void add_edges(
  const std::vector<std::vector<double>>& attractiveness,
  genotype_graph& g,
  const simulation_parameters& parameters
);

///Add edges between genotypes for attractivenesses above a certain minimum
/// @param attractiveness attractiveness ordered that v[observant][observed] equals the amount
///   the observant is attracted to the observed
void add_edges(
  const std::vector<std::vector<double>>& attractiveness,
  genotype_graph& g,
  const double minimal_attractiveness
);

void add_vertexes(
  const individuals& pop,
  genotype_graph& g
);


} //~namespace kewe

#endif // KEWE_GENOTYPE_GRAPH_H
