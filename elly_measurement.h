#ifndef ELLY_MEASUREMENT_H
#define ELLY_MEASUREMENT_H

#include "elly_event_rates.h"
#include "elly_location.h"
#include "elly_populations.h"

namespace elly {

///Measurements done at every timestep in the simulation
class measurement
{
public:
  ///@param t time at which the measurement is done
  ///@param r the event rates at that point in time
  measurement(
    const double t,
    const populations& p,
    const event_rates& r
  );

  const auto& get_event_rates() const noexcept { return m_r; }
  const auto& get_populations() const noexcept { return m_p; }
  double get_time() const noexcept { return m_t; }

private:
  const populations m_p;
  const event_rates m_r;
  const double m_t;
};

///Count the number of extinct species for that measurement
int count_extinct(const measurement& m);

///Count the number of species at a certain location
int count_is_on(const measurement& m, const location any_location) noexcept;

} //~namespace elly

#endif // ELLY_MEASUREMENT_H
