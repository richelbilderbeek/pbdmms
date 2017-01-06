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

std::vector<elly::rate> elly::collect(
  const events_rates_in_time& v,
  const event e
)
{
  std::vector<rate> w;
  w.reserve(v.size());
  std::transform(
    std::begin(v),
    std::end(v),
    std::back_inserter(w),
    [e](const auto& p)
    {
      return p.second.get(e);
    }
  );
  return w;
}

std::vector<double> elly::to_raw(const std::vector<rate>& v) noexcept
{
  std::vector<double> w;
  w.reserve(v.size());
  std::transform(
    std::begin(v),
    std::end(v),
    std::back_inserter(w),
    [](const auto r)
    {
      return r.get();
    }
  );
  return w;
}
