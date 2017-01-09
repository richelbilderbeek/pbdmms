#include "elly_measurement.h"

elly::measurement::measurement(
  const double t,
  const populations& p,
  const event_rates& r
)
  :
    m_p{p},
    m_r{r},
    m_t{t}
{
}

int elly::count_extinct(const measurement& m)
{
  return count_extinct(m.get_populations());
}

int elly::count_is_on(const measurement& m, const location any_location) noexcept
{
  return count_is_on(m.get_populations(), any_location);
}

