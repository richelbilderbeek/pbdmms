#include "elly_gillespie.h"
#include "elly_event_rates.h"
#include "elly_parameters.h"
#include "elly_events.h"
#include "elly_simulation.h"
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

void elly::do_nth_event(
  const int e,
  const parameters& p,
  simulation& s,
  const double time,
  std::mt19937& rng
)
{
  switch(e) {
    case 0: mainland_cladogenesis(s, time);
      break;
    case 1: mainland_extinction(all_species_mainland, extinct_species, p, time);
      break;
    case 2: mainland_immigration(all_species_mainland, all_species_both, p, species_in_clade);
      break;
    case 3: island_extinction(all_species_island, extinct_species, p, time, species_in_clade);
      break;
    case 4: island_cladogenesis(all_species_island, extinct_species,p, time, species_in_clade);
      break;
    case 5: island_immigration(all_species_island, all_species_both, p);
      break;
    case 6: both_extinction_mainland(all_species_both, all_species_island, p);
      break;
    case 7: both_extinction_island(all_species_both, all_species_mainland, p, species_in_clade);
      break;
    case 8: both_anagenesis(all_species_mainland, all_species_island, all_species_both, p, time);
      break;
    case 9: both_cladogenesis_island(all_species_mainland, all_species_island, all_species_both, p, time, species_in_clade);
      break;
    case 10: both_cladogenesis_mainland(all_species_mainland, all_species_island, all_species_both, p, time);
      break;
    default:
      throw std::logic_error("drawn event that does not exist");
    }
}


//Still have to resolve drawing diversity dependant events
int elly::draw_dd_event(
  const std::vector<int>& species_in_clades,
  event_rates& r,
  const parameters& p,
  const int io,
  const int bo,
  const int mo
)
{
  calculate_rates_per_clade(
    species_in_clades,
    p,
    r ,
    io,
    bo,
    mo
  );

  std::vector<double> temp_bcladi = r.get_dd_rates_bcladi();
  std::vector<double> temp_mimm = r.get_dd_rates_mimm();
  std::vector<double> temp_iclad = r.get_dd_rates_iclad();

  std::discrete_distribution<int> event_num(temp_bcladi.begin(), temp_bcladi.end());
  std::discrete_distribution<int> event_num2(temp_mimm.begin(), temp_mimm.end());
  std::discrete_distribution<int> event_num3(temp_iclad.begin(), temp_iclad.end());

  std::mt19937_64 rng;
  rng.seed(p.get_rng_seed());

  event_num2(rng);
  event_num3(rng);
  return event_num(rng);
}

/*int draw_dd_event_mimm(elly::variables v,
                       const elly::rates& r,
                       const elly::parameters& p)
{
  calculate_rates_per_clade()
}
*/
