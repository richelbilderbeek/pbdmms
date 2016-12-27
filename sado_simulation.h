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

///@param as: attractivenesses
[[deprecated]]
offspring create_kids(
  const population& pop,
  const my_iterator i,
  const std::vector<double>& as
);

///@param as: attractivenesses
offspring create_kids(
  const population& pop,
  const indiv& mother,
  const std::vector<double>& as
);

void do_simulation(const std::string& filename);

population create_initial_population(
  const parameters& p
);


void iterate(population pop, const parameters& p);

int pick_random_individual_index(
  const int pop_size
);

std::vector<indiv>::iterator find_nth_individual(
  std::vector<indiv>& pop,
  const int n
);

std::list<indiv>::iterator find_nth_individual(
  std::list<indiv>& pop,
  const int n
);

indiv get_nth_individual(
  const population& pop,
  const int n
);

[[deprecated]]
my_iterator randomindividual(
  population& pop,
  const int pop_size
);

[[deprecated]]
std::vector<double> get_attractivenesses(
  const population& pop,
  const my_iterator i,
  const double pi,
  const double xi
);

std::vector<double> get_attractivenesses(
  const population& pop,
  const double pi,
  const double xi
);

[[deprecated]]
std::vector<double> get_summed_attractivenesses(
  const population& pop,
  const my_iterator i,
  const double pi,
  const double xi
);

} //~namespace sado

#endif // SADO_SIMULATION_H
