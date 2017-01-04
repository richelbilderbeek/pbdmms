#include "elly_gillespie.h"
#include "elly_event_rates.h"
#include "elly_events.h"
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
  const rate sum_rates{calc_sumrates(r)};
  std::exponential_distribution<double> waiting_time(sum_rates.get());
  return waiting_time(rng);
}

elly::event elly::draw_event(
  const event_rates& r,
  std::mt19937& rng
)
{
  const std::map<event, rate> m = collect_rates(r);

  //Extract the rates (as double) and events, both in their own vectors
  std::vector<double> rates;
  for(const auto& p: m)
  {
    rates.push_back(p.second.get());
  }
  std::vector<event> events;
  for(const auto& p: m)
  {
    events.push_back(p.first);
  }
  assert(events.size() == rates.size());

  //Draw an event from those doubles in 'rates'
  std::discrete_distribution<int> event_indices(std::begin(rates), std::end(rates));
  const int event_index = event_indices(rng);

  //From the map, get the event_index'th event
  assert(event_index >= 0);
  assert(event_index < static_cast<int>(events.size()));
  return events[event_index];
}

void elly::do_event(
  const event_rates& r,
  simulation& s
)
{
  const event e = draw_event(r, s.get_rng());
  do_event(e, s);
}

void elly::do_event( //!OCLINT Cannot be simpler
  const event e,
  simulation& s
)
{
  switch(e)
  {
    case event::ana: both_anagenesis(s); break;
    case event::clad_glob_on_island: cladogenesis_global_on_island(s); break;
    case event::clad_glob_on_main: cladogenesis_global_on_mainland(s); break;
    case event::clad_island_only: cladogenesis_island_only(s); break;
    case event::clad_main_only: cladogenesis_mainland_only(s); break;
    case event::ext_glob_on_island: both_extinction_island(s); break;
    case event::ext_glob_on_main: both_extinction_mainland(s); break;
    case event::ext_island_only: island_extinction(s); break;
    case event::ext_main_only: mainland_extinction(s); break;
    case event::migration_to_island: mainland_immigration(s); break;
    /*
    case 0;: cladogenesis_mainland_only(s); break;
    case 1: mainland_extinction(s); break;
    case 2: mainland_immigration(s); break;
    case 3: island_extinction(s); break;
    case 4: cladogenesis_island_only(s); break;
    case 5: both_extinction_mainland(s);  break;
    case 6: both_extinction_island(s); break;
    case 7:
    case 8: cladogenesis_global_on_island(s); break;
    case 9: cladogenesis_global_on_mainland(s); break;
    default: throw std::logic_error("drawn event that does not exist");
    */
  }
}
