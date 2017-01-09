#ifndef ELLY_GILLESPIE_H
#define ELLY_GILLESPIE_H

#include "elly_fwd.h"
#include "elly_events.h"

#include <random>

namespace elly {


///When will the next event take place?
double draw_waiting_time(
  const event_rates& r,
  std::mt19937& rng
);

///When an event happens, which one takes place?
event draw_event(
  const event_rates& r,
  std::mt19937& rng
);

///Let an event take place
void do_event(
  const event_rates& r,
  simulation& s
);

///Let an event take place
void do_event(
  const event n,
  simulation& s
);

///Let an event take place on a desired species
void do_event(
  const event n,
  const species& s,
  simulation& sim
);

} //~namespace elly

#endif // ELLY_GILLESPIE_H
