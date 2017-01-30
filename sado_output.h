#ifndef SADO_OUTPUT_H
#define SADO_OUTPUT_H

#include "sado_fwd.h"
#include "sado_histogram.h"
#include "sado_population.h"

namespace sado
{

/// Appends histogram to file
void append_histogram(const histogram &p, const std::string &filename);

void output(
    const population &pop, const int t, const parameters &p, results &r);

} //~namespace sado

#endif // SADO_OUTPUT_H
