#ifndef SADO_SIMULATION_H
#define SADO_SIMULATION_H

#include <string>
#include <vector>
#include "sado_individual.h"
#include "sado_parameters.h"
#include "sado_population.h"

namespace sado {

///Appends histogram to file
//Use Stroustrup's least favorite interface :-(
void append_histogram(const double * const p, const int sz, const std::string& filename);

double calc_comp(
  const population& p,
  const double xi
) noexcept;

offspring create_kids(
  const population& pop,
  const double attractiveness,
  const my_iterator i
);

void do_simulation(const std::string& filename);

double gauss(double xx, double sigma);

std::vector<std::string> get_golden_output() noexcept;

population initialize(
  const parameters& p
);

///Optimistic comparison
bool is_more_or_less_same(
  const std::vector<double>& v,
  const std::vector<double>& w
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

sado::parameters readparameters(const std::string& filename);

std::vector<std::string> seperate_string(
  const std::string& input,
  const char seperator
);

double set_and_sum_attractivenesses(
  population& pop,
  const my_iterator i,
  const double pi,
  const double xi
);

std::vector<double> to_doubles(
  const std::vector<std::string>& v
);

} //~namespace sado

#endif // SADO_SIMULATION_H
