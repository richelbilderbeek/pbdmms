#include "elly_events_rates_in_time.h"

#include <algorithm>

std::vector<double> elly::collect_ts(const events_rates_in_time& v) noexcept
{
  std::vector<double> w;
  w.reserve(v.size());
  std::transform(
    std::begin(v),
    std::end(v),
    std::back_inserter(w),
    [](const auto& p)
    {
      return p.first;
    }
  );
  return w;
}


