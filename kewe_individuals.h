#ifndef KEWE_INDIVIDUALS_H
#define KEWE_INDIVIDUALS_H

#include <vector>
#include "kewe_individual.h"

namespace kewe {

using individuals = std::vector<indiv>;

///Calculate the survivabilities of the individuals in the
///population. This is dependent on the ecological trait
///and its distance from the ecological optimum
std::vector<double> calc_survivabilities(const individuals& pop);


} //~namespace kewe

#endif // KEWE_INDIVIDUALS_H
