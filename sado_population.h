#ifndef SADO_POPULATION_H
#define SADO_POPULATION_H

//#define SADO_USE_LIST
//#include <list>
#include <vector>

#include "sado_individual.h"

namespace sado {

using offspring = std::vector<indiv>;

#ifdef SADO_USE_LIST
using population = std::list<indiv>;
#else
using population = std::vector<indiv>;
#endif

double get_mean_x(const population& p);
double get_mean_p(const population& p);
double get_mean_q(const population& p);

} //namespace sado

#endif // SADO_POPULATION_H
