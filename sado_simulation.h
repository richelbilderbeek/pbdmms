#ifndef SADO_SIMULATION_H
#define SADO_SIMULATION_H

#include <string>
#include <vector>
#include "sado_individual.h"
#include "sado_parameters.h"
#include "sado_population.h"

namespace sado {

double calc_comp(
  const population& p,
  const double xi
) noexcept;

///@param sum_a: the sum of all attractivenesses
offspring create_kids(
  const population& pop,
  const my_iterator i,
  const std::vector<double>& as
);

void do_simulation(const std::string& filename);

population initialize(
  const parameters& p
);


void iterate(population& pop, const parameters& p);

int pick_random_individual_index(
  const int pop_size
);


my_iterator randomindividual(
  population& pop,
  const int pop_size
);


std::vector<double> get_summed_attractivenesses(
  const population& pop,
  const my_iterator i,
  const double pi,
  const double xi
);

std::vector<double> get_attractivenesses(
  const population& pop,
  const my_iterator i,
  const double pi,
  const double xi
);


} //~namespace sado

#endif // SADO_SIMULATION_H
