#ifndef SADO_SIMULATION_H
#define SADO_SIMULATION_H

#include <string>
#include <vector>
#include "sado_individual.h"
#include "sado_population.h"

namespace sado {

///Appends histogram to file
//Use Stroustrup's least favorite interface :-(
void append_histogram(const double * const p, const int sz, const std::string& filename);

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

///Optimistic comparison
bool is_more_or_less_same(
  const std::vector<double>& v,
  const std::vector<double>& w
);

void iterate();

void output(bigint t);

my_iterator randomindividual();

void readparameters(const std::string& filename);

std::vector<std::string> seperate_string(
  const std::string& input,
  const char seperator
);

double set_and_sum_attractivenesses(
  const my_iterator i,
  const double pi,
  const double xi
);


my_iterator start();

std::vector<double> to_doubles(
  const std::vector<std::string>& v
);

} //~namespace sado

#endif // SADO_SIMULATION_H
