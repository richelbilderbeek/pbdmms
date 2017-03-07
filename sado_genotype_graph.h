#ifndef SADO_GENOTYPE_GRAPH_H
#define SADO_GENOTYPE_GRAPH_H

#include <boost/graph/adjacency_list.hpp>

#include "sado_simulation.h"
#include "sado_fwd.h"
#include "sado_individual.h"
#include "sado_attractiveness_matrix.h"
namespace sado {

using genotype_graph
  = boost::adjacency_list<
      boost::vecS, boost::vecS, boost::undirectedS, std::string
    >;

using attractiveness_matrix = std::vector<std::vector<double>>;

///Add edges between genotypes for attractivenesses above a certain minimum
/// @param attractiveness attractiveness ordered that v[observant][observed] equals the amount
///   the observant is attracted to the observed
void add_edges(
  const attractiveness_matrix& as,
  genotype_graph& g,
  const parameters& p
);

///Add edges between genotypes for attractivenesses above a certain minimum
/// @param attractiveness attractiveness ordered that v[observant][observed] equals the amount
///   the observant is attracted to the observed
/// @param min_attractiveness the minimum attractiveness to make a female willing to mate
void add_edges(
  const attractiveness_matrix& as,
  genotype_graph& g,
  const double min_attractiveness
);

void add_edges_impl_1(
  const attractiveness_matrix& as,
  genotype_graph& g,
  const double min_attractiveness
);

void add_edges_impl_2(
  const attractiveness_matrix& as,
  genotype_graph& g,
  const double min_attractiveness
);

void add_vertices(
  const population& pop,
  genotype_graph& g
);

bool is_valid(const attractiveness_matrix& as) noexcept;


} //~namespace sado

#endif // SADO_GENOTYPE_GRAPH_H
