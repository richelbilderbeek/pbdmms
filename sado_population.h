#ifndef SADO_POPULATION_H
#define SADO_POPULATION_H

#define SADO_USE_LIST
#ifdef SADO_USE_LIST
#include <list>
#else
#include <vector>
#endif

#include "sado_individual.h"

namespace sado {

#ifdef SADO_USE_LIST
using population = std::list<indiv>;
#else
using population = std::vector<indiv>;
#endif
using my_iterator = population::iterator;

} //namespace sado

#endif // SADO_POPULATION_H
