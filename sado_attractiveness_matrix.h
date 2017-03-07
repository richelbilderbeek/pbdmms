#ifndef SADO_ATTRACTIVENESS_MATRIX_H
#define SADO_ATTRACTIVENESS_MATRIX_H

#include <string>
#include <vector>

#include "sado_parameters.h"
#include "sado_population.h"

namespace sado {


/// A matrix containing the attractivenesses the indivuals
/// have towards each other
/// For example:
///      [0]  [1]
/// [0]  0.0  0.5
/// [1]  0.2  0.0
/// Individuals do not find themselves attractive
using attractiveness_matrix = std::vector<std::vector<double>>;

attractiveness_matrix create_attractiveness_matrix(
  const population& pop, const parameters& p);

bool is_valid(const attractiveness_matrix& as) noexcept;

} //~namespace sado

#endif // SADO_ATTRACTIVENESS_MATRIX_H
