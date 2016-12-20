#ifndef ELLY_GILLESPIE_H
#define ELLY_GILLESPIE_H
#include "elly_event_rates.h"
#include "elly_parameters.h"
#include "elly_species.h"
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
  std::vector<species>& all_species_mainland,
  std::vector<species>& all_species_island,
  std::vector<species>& all_species_both,
  const parameters& p,
  std::vector<species>& extinct,
  std::vector<species_id>& species_in_clades,
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
