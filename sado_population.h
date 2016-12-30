#ifndef SADO_POPULATION_H
#define SADO_POPULATION_H

#include <vector>

#include "sado_individual.h"

namespace sado {

using offspring = std::vector<indiv>;
using population = std::vector<indiv>;

double get_mean_x(const population &p);
double get_mean_p(const population &p);
double get_mean_q(const population &p);

} // namespace sado

#endif // SADO_POPULATION_H
