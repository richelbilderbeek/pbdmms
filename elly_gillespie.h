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

///Let the nth event take place
void do_nth_event(
  const int n,
  const parameters& p,
  simulation& s,
  const double time,
  std::mt19937& rng
);

int draw_dd_event(
  const std::vector<int>& species_in_clades,
  event_rates& r,
  const parameters& p,
  const int io,
  const int bo,
  const int mo
);

} //~namespace elly

#endif // ELLY_GILLESPIE_H
