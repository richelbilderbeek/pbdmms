#ifndef SADO_OUTPUT_H
#define SADO_OUTPUT_H

#include "sado_population.h"
#include "sado_fwd.h"
#include "sado_histogram.h"

namespace sado {

///Appends histogram to file
void append_histogram(
  const histogram& p,
  const std::string& filename
);

void create_header(const parameters& p);

std::vector<std::string> get_golden_output() noexcept;

void output(
  const population& pop,
  const int t,
  const parameters& p,
  results& r
);


} //~namespace sado

#endif // SADO_OUTPUT_H
