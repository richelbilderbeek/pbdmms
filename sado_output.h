#ifndef SADO_OUTPUT_H
#define SADO_OUTPUT_H

#include <map>
#include <vector>
#include "sado_fwd.h"
#include "sado_mate_graph.h"
#include "sado_histogram.h"
#include "sado_population.h"

namespace sado
{

/// Appends histogram to file
void append_histogram(const histogram &p, const std::string &filename);

void output(
    const population &pop, const int t, const parameters &p, results &r);

void copy_indivs_to_species(
  const population& pop, const int gen, results& r, const parameters& p);

///Copy all individuals in the mate graph, seperated
///per ID.
/// @param ids specied IDs, each value must be zero at least, for example,
///   as created by boost::connected_components
/// @param mate_graph a graph that connects individuals that can potentially mate
/*
std::map<int, std::vector<indiv>> seperate_individuals_by_id(
  std::vector<int> ids,
  const mate_graph& g
);
*/

} //~namespace sado

#endif // SADO_OUTPUT_H
