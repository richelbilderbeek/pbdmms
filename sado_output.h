#ifndef SADO_OUTPUT_H
#define SADO_OUTPUT_H

#include "sado_population.h"

namespace sado {

///Appends histogram to file
void append_histogram(const std::vector<double>& p, const std::string& filename);

void create_header();

std::vector<std::string> get_golden_output() noexcept;

void output(
  const population& pop,
  int t,
  const int pop_size
);


} //~namespace sado

#endif // SADO_OUTPUT_H
