#ifndef KEWE_INDIVIDUALS_H
#define KEWE_INDIVIDUALS_H

#include <vector>
#include "kewe_individual.h"

namespace kewe {

using individuals = std::vector<individual>;



std::vector<double> calc_competitivenesses(
  const individuals& pop,
  const parameters& p
);

///Calculate the survivabilities of the individuals in the population
std::vector<double> calc_survivabilities(
  const individuals& pop,
  const parameters& parameters
);

///Used by calc_survivabilities
std::vector<double> calc_survivabilities(
  const individuals& pop,
  const std::vector<double>& pop_comp,
  const parameters& parameters
);


} //~namespace kewe

#endif // KEWE_INDIVIDUALS_H
