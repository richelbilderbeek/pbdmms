#ifndef SADO_SIMULATION_H
#define SADO_SIMULATION_H

#include <vector>
#include "sado_individual.h"
#include "sado_population.h"

namespace sado {

double calc_comp(
  const my_iterator i,
  const double xi
) noexcept;

void create_kids(
  const double attractiveness,
  const my_iterator i
);

void do_simulation(const std::string& filename);

my_iterator end();

double gauss(double xx, double sigma);

std::vector<std::string> get_golden_output() noexcept;

void initialize();

void iterate();

void output(bigint t);

my_iterator randomindividual();

void readparameters(const std::string& filename);

double set_and_sum_attractivenesses(
  const my_iterator i,
  const double pi,
  const double xi
);

///Appends histogram to file
//Use Stroustrup's least favorite interface :-(
void append_histogram(const double * const p, const int sz, const std::string& filename);

my_iterator start();

} //~namespace sado

#endif // SADO_SIMULATION_H
