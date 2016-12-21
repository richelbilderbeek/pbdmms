#ifndef KEWE_INDIVIDUALS_H
#define KEWE_INDIVIDUALS_H

#include <vector>
#include "kewe_individual.h"

namespace kewe {

using individuals = std::vector<individual>;

std::vector<double> calc_competitivenesses(
  const individuals& pop,
  const simulation_parameters& p
);

///Calculate the survivabilities of the individuals in the population
std::vector<double> calc_survivabilities(
  const individuals& pop,
  const simulation_parameters& parameters
);

///Used by calc_survivabilities
std::vector<double> calc_survivabilities(
  const individuals& pop,
  const std::vector<double>& pop_comp,
  const simulation_parameters& parameters
);

///Creates a population with these phenotypes:
/// +---+-------------------+------------+---------+
/// | # | female_preference | male_trait | ecotype |
/// +---+-------------------+------------+---------+
/// | a |       1.0         |    1.0     |   1.0   |
/// | b |       1.0         |    2.0     |   1.0   |
/// | c |       1.0         |    3.0     |   1.0   |
/// +---+-------------------+------------+---------+
individuals create_test_individuals_1() noexcept;

} //~namespace kewe

#endif // KEWE_INDIVIDUALS_H
