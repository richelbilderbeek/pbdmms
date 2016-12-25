#ifndef SADO_POPULATION_H
#define SADO_POPULATION_H

#include <list>
#include "sado_individual.h"

namespace sado {

using population = std::list<indiv>;
extern population pop; //GLOBAL
using  my_iterator = population::iterator;

} //namespace sado

#endif // SADO_POPULATION_H
