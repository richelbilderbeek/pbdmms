#ifndef ELLY_EVENTS_RATES_IN_TIME_H
#define ELLY_EVENTS_RATES_IN_TIME_H

#include <vector>
#include "elly_event_rates.h"

namespace elly {

using events_rates_in_time = std::vector<std::pair<double, event_rates>>;

std::vector<double> collect_ts(const events_rates_in_time& v) noexcept;

std::vector<rate> collect(
  const events_rates_in_time& v,
  const event e
);

std::vector<double> to_raw(const std::vector<rate>& v) noexcept;

} //~namespace elly

#endif // ELLY_EVENTS_RATES_IN_TIME_H
