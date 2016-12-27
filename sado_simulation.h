#ifndef SADO_SIMULATION_H
#define SADO_SIMULATION_H

#include <string>
#include <vector>
#include "sado_individual.h"
#include "sado_parameters.h"
#include "sado_population.h"

namespace sado {

///Appends histogram to file
void append_histogram(const std::vector<double>& p, const std::string& filename);

double calc_comp(
  const population& p,
  const double xi
) noexcept;

///@param sum_a: the sum of all attractivenesses
offspring create_kids(
  const population& pop,
  const double sum_a,
  const my_iterator i
);

void do_simulation(const std::string& filename);

population initialize(
  const parameters& p
);


void iterate(population& pop, const parameters& p);

void output(
  const population& pop,
  bigint t,
  const int pop_size
);

my_iterator randomindividual(
  population& pop,
  const int pop_size
);


double set_and_sum_attractivenesses(
  population& pop,
  const my_iterator i,
  const double pi,
  const double xi
);


} //~namespace sado

#endif // SADO_SIMULATION_H
