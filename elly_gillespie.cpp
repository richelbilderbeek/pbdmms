#include "elly_gillespie.h"
#include "elly_event_rates.h"
#include "elly_parameters.h"
#include "elly_events.h"
#include "elly_simulation.h"

#include <cassert>
#include <random>
#include <cstdlib>

double elly::draw_waiting_time(
  const event_rates& r,
  std::mt19937& rng
)
{
  const double sum_rates{calc_sumrates(r)};
  std::exponential_distribution<double> waiting_time(sum_rates);
  return waiting_time(rng);
}

int elly::draw_event(
  const event_rates& r,
  std::mt19937& rng
)
{
  const std::vector<double> rates = to_ratesvector(r);
  std::discrete_distribution<> event_indices(std::begin(rates), std::end(rates));
  return event_indices(rng);
}

void elly::do_event(
  const event_rates& r,
  simulation& s
)
{
  const int n{draw_event(r, s.get_rng())};
  assert(n > 0);
  assert(n < 10);
  do_nth_event(n, s);
}

void elly::do_nth_event(
  const int e,
  simulation& s
)
{
  assert(e > 0);
  assert(e < 10);

  switch(e)
  {
    case 0: mainland_cladogenesis(s); break;
    case 1: mainland_extinction(s); break;
    case 2: mainland_immigration(s); break;
    case 3: island_extinction(s); break;
    case 4: island_cladogenesis(s); break;
    case 5: both_extinction_mainland(s);  break;
    case 6: both_extinction_island(s); break;
    case 7: both_anagenesis(s); break;
    case 8: both_cladogenesis_island(s); break;
    case 9: both_cladogenesis_mainland(s); break;
    default: throw std::logic_error("drawn event that does not exist");
  }
}
