#ifndef SADO_POPULATION_H
#define SADO_POPULATION_H

#define SADO_USE_LIST
#include <list>
#include <vector>

#include "sado_individual.h"

namespace sado {

using offspring = std::vector<indiv>;

#ifdef SADO_USE_LIST
using population = std::list<indiv>;
#else
using population = std::vector<indiv>;
#endif
using my_iterator = population::iterator;

} //namespace sado

#endif // SADO_POPULATION_H
