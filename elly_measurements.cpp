#include "elly_measurements.h"

#include <algorithm>

std::vector<double> elly::collect_ts(const measurements& v) noexcept
{
  std::vector<double> w;
  w.reserve(v.size());
  std::transform(
    std::begin(v),
    std::end(v),
    std::back_inserter(w),
    [](const auto& m)
    {
      return m.get_time();
    }
  );
  return w;
}

std::vector<elly::rate> elly::collect(
  const measurements& v,
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
      return p.get_event_rates().get(e);
    }
  );
  return w;
}

std::vector<int> elly::collect(
  const measurements& v,
  const location any_location
)
{
  std::vector<int> w;
  w.reserve(v.size());
  std::transform(
    std::begin(v),
    std::end(v),
    std::back_inserter(w),
    [any_location](const measurement& m)
    {
      return count_is_on(m, any_location);
    }
  );
  return w;
}

std::vector<int> elly::collect_extinct(
  const measurements& v
)
{
  std::vector<int> w;
  w.reserve(v.size());
  std::transform(
    std::begin(v),
    std::end(v),
    std::back_inserter(w),
    [](const measurement& m)
    {
      return count_extinct(m);
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
