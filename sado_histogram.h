#ifndef SADO_HISTOGRAM_H
#define SADO_HISTOGRAM_H

#include <vector>
#include "sado_fwd.h"
#include "sado_population.h"

namespace sado {

using histogram = std::vector<double>;

histogram create_histogram_p(const population& pop, const parameters& p);
histogram create_histogram_q(const population& pop, const parameters& p);
histogram create_histogram_x(const population& pop, const parameters& p);

///Rescale a histogram in such a way, that its heighest
///becomes 1.0
histogram rescale_max_to_one(histogram h);


} //~namespace sado

#endif // SADO_HISTOGRAM_H
