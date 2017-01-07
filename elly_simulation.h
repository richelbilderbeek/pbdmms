#ifndef ELLY_SIMULATION_H
#define ELLY_SIMULATION_H

#include <iosfwd>
#include <random>
#include <vector>
#include "elly_fwd.h"
#include "elly_events.h"
#include "elly_parameters.h"
#include "elly_populations.h"
#include "elly_results.h"
#include "elly_event_rates.h"
#include "elly_measurement.h"

namespace elly {

class simulation
{
public:
  simulation(const parameters& p);

  void add_extinct(const species& s) { m_populations.add_species(s); }

  ///Count the number of species that only occur on a location
  int count_species(const location where) const noexcept;

  ///Count the number of species in a certain clade
  int count_species(const clade_id& id) const noexcept;

  ///Progress until the next event takes place, and
  ///do that event.
  void do_next_event();

  ///Progress time for a certain amount of time,
  ///then do the requested event
  /// @param t_to_event time to pass before the next event takes place, in million of years
  /// @param e the event to take place after t_to_event million of years
  void do_next_event(const double t_to_event, const event e);

  ///Gets and removes a random species present in the location specified
  ///It is up to the client to put it someplace else
  species extract_random_species(const location any_location);

  const auto& get_measurements() const noexcept { return m_measurements; }

  populations& get_populations() noexcept { return m_populations; }
  const populations& get_populations() const noexcept { return m_populations; }

  double get_time() const noexcept { return m_t; }

  std::mt19937& get_rng() noexcept { return m_rng; }

  void run();

  private:

  ///The event rates at each point in time
  std::vector<measurement> m_measurements;

  const parameters m_parameters;

  populations m_populations;

  std::mt19937 m_rng;

  ///The current time of the simulation, in million years
  double m_t;

};

///adds two new mainland species from the same clade as the parent species,
///also pushes parent species from mainland species vector to extinct species vector
void cladogenesis_mainland_only(simulation& s);

///pushes random species from mainland species vector to extinct species vector
void mainland_extinction(simulation &s);

///removes species from mainland species vector and adds it to both (mainland and island) vector
void mainland_immigration(simulation &s);

///pushes random species from island species vector to extinct species vector
void island_extinction(simulation &s);

//adds two new island species from the same clade as the parent species,
//also pushes parent species from island species vector to extinct species vector
void cladogenesis_island_only(simulation &s);

#ifdef ALLOW_COLONIZATION_OF_MAINLAND_FROM_ISLAND
//removes species from island species vector and adds it to both habitats species vector
void island_immigration(simulation &s);
#endif // ALLOW_COLONIZATION_OF_MAINLAND_FROM_ISLAND

//removes species from both habitats species vector and adds it to mainland species vector
void both_extinction_island(simulation &s);

//removes species from both habitats species vector and adds it to island species vector
void both_extinction_mainland(simulation &s);

//removes species from both habitats species vector and adds it to mainland species vector,
//also creating a new species in island species vector
void both_anagenesis(simulation &s);

//removes species from both habitats species vector and adds it to mainland species vector,
//also creates two new species in island species vector
void cladogenesis_global_on_island(simulation &s);

//removes species from both habitats species vector and adds it to island species vector,
//also creates two new species in mainland species vector
void cladogenesis_global_on_mainland(simulation &s);

} //~namespace elly

#endif // ELLY_SIMULATION_H
