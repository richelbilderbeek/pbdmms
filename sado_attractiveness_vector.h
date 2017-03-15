#ifndef SADO_ATTRACTIVENESS_VECTOR_H
#define SADO_ATTRACTIVENESS_VECTOR_H

#include <vector>
#include "sado_fwd.h"

namespace sado {

using attractiveness_vector = std::vector<double>;

///Get the attractivenesses of each individual in a population
///for an individual with a certain preference and ecological trait
attractiveness_vector get_attractivenesses(
  const population &pop,
  const double preference,
  const double eco_trait,
  const parameters &p);

} //~namespace sado

#endif // SADO_ATTRACTIVENESS_VECTOR_H
