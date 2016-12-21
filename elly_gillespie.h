#ifndef ELLY_GILLESPIE_H
#define ELLY_GILLESPIE_H

#include "elly_fwd.h"

#include <random>

namespace elly {


///When will the next event take place?
double draw_waiting_time(
  const event_rates& r,
  std::mt19937& rng
);

///When an event happens, which one takes place?
int draw_event(
  const event_rates& r,
  std::mt19937& rng
);

///Let an event take place
void do_event(
  const event_rates& r,
  simulation& s,
  const double time
);

///Let the nth event take place
void do_nth_event(
  const int n,
  simulation& s,
  const double time
);

} //~namespace elly

#endif // ELLY_GILLESPIE_H
