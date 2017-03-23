#ifndef SADO_OUTPUT_H
#define SADO_OUTPUT_H

#include <map>
#include <vector>
#include "sado_fwd.h"
#include "sado_histogram.h"
#include "sado_population.h"
#include "sado_result.h"
#include "sado_species.h"

namespace sado
{

/// Appends histogram to file
void append_histogram(const histogram &p, const std::string& filename);

/// Measure the population
result measure(
  const population& pop,
  const int t,
  const parameters& p
);

///Take the individuals in the population and group these
///into species
std::vector<species> group_individuals_to_species(
  const population& pop,
  const parameters& p,
  const int gen //to be removed
);

} //~namespace sado

#endif // SADO_OUTPUT_H
