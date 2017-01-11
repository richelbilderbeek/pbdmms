#ifndef ELLY_MEASUREMENTS_H
#define ELLY_MEASUREMENTS_H

#include <vector>
#include "elly_location.h"
#include "elly_measurement.h"

namespace elly {

using measurements = std::vector<measurement>;

std::vector<double> collect_ts(const measurements& v) noexcept;

///Collect all the rates for a certain event
std::vector<rate> collect(
  const measurements& v,
  const event e
);

///Collect all the population sizes for a certain location
std::vector<int> collect(
  const measurements& v,
  const location any_location
);

///Collect the number of extinct species
std::vector<int> collect_extinct(
  const measurements& v
);


std::vector<double> to_raw(const std::vector<rate>& v) noexcept;

} //~namespace elly

#endif // ELLY_MEASUREMENTS_H
