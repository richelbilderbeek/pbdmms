#ifndef ELLY_EVENTS_RATES_IN_TIME_H
#define ELLY_EVENTS_RATES_IN_TIME_H

#include <vector>
#include "elly_event_rates.h"

namespace elly {

using events_rates_in_time = std::vector<std::pair<double, event_rates>>;

std::vector<double> collect_ts(const events_rates_in_time& v) noexcept;

} //~namespace elly

#endif // ELLY_EVENTS_RATES_IN_TIME_H
