#include "elly_gillespie.h"
#include "elly_rates.h"
#include "elly_parameters.h"
#include "elly_eventfunctions.h"
#include <random>
#include <cstdlib>






double draw_waiting_time(const double sumrates, const elly::parameters& p)
{
  std::mt19937_64 rng;
  rng.seed(p.get_rng_seed());

  std::exponential_distribution<double> waiting_time(sumrates);
   return waiting_time(rng);

}


int draw_event(const elly::rates& r , const elly::parameters& p)
{
  std::vector<double> rates = elly::to_ratesvector(r);
  std::discrete_distribution<int> event_num(rates.begin(), rates.end());

  std::mt19937_64 rng;
  rng.seed(p.get_rng_seed());
  return event_num(rng);
}

void pick_species(int e,
                  std::vector<elly::species> all_species_mainland,
                  std::vector<elly::species> all_species_island,
                  std::vector<elly::species> all_species_both,
                  elly::parameters p,
                  std::vector<elly::species> extinct_species,
                  int id_counter,
                  std::vector<int> species_in_clade,
                  double time)
{
  switch(e){
    case 0: mainland_cladogenesis(all_species_mainland,
                                  extinct_species,
                                  p,
                                  id_counter,
                                  time);
      break;
    case 1: mainland_extinction(all_species_mainland,
                                extinct_species,
                                p,
                                time);
      break;
    case 2: mainland_immigration(all_species_mainland,
                                 all_species_both,
                                 p,
                                 species_in_clade);
      break;
    case 3: island_extinction(all_species_island,
                              extinct_species,
                              p,
                              time,
                              species_in_clade);
      break;
    case 4: island_cladogenesis(all_species_island,
                                extinct_species,
                                p,
                                id_counter,
                                time,
                                species_in_clade);
      break;
    case 5: island_immigration(all_species_island,
                               all_species_both,
                               p);
      break;
    case 6: both_extinction_mainland(all_species_both,
                                     all_species_island,
                                     p);
      break;
    case 7: both_extinction_island(all_species_both,
                                   all_species_mainland,
                                   p,
                                   species_in_clade);
      break;
    case 8: both_anagenesis(all_species_mainland,
                            all_species_island,
                            all_species_both,
                            p,
                            time,
                            id_counter);
      break;
    case 9: both_cladogenesis_island(all_species_mainland,
                                     all_species_island,
                                     all_species_both,
                                     p,
                                     time,
                                     id_counter,
                                     species_in_clade);
      break;
    case 10: both_cladogenesis_mainland(all_species_mainland,
                                        all_species_island,
                                        all_species_both,
                                        p,
                                        time,
                                        id_counter);
      break;
    default:
      throw std::logic_error("drawn event that does not exist");
    }
}
int draw_dd_event(std::vector<int> species_in_clades,
                  const elly::parameters& p,
                  int io, int bo, int mo)
{
  std::vector<double> dd_rates_mimm;
  std::vector<double> dd_rates_iclad;
  std::vector<double> dd_rates_bcladi;

  calculate_rates_per_clade(species_in_clades, p, dd_rates_mimm,
                            dd_rates_iclad, dd_rates_bcladi, io, bo, mo);

  std::discrete_distribution<int> event_num(dd_rates_bcladi.begin(), dd_rates_bcladi.end());

  std::mt19937_64 rng;
  rng.seed(p.get_rng_seed());
  return event_num(rng);
}
