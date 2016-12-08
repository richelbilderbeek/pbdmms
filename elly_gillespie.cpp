#include "elly_gillespie.h"
#include "elly_rates.h"
#include "elly_parameters.h"
#include <random>
#include <cstdlib>






double draw_waiting_time(const elly::rates& r, const elly::parameters& p)
{
  std::mt19937_64 rng;
  rng.seed(p.get_rng_seed());

  std::exponential_distribution<double> waiting_time(calc_sumrates(r));
  double wt = waiting_time(rng);
  return wt;
}


int draw_event(const elly::rates& r , const elly::parameters& p)
{
  std::vector<double> rates = elly::to_ratesvector(r);
  std::discrete_distribution<int> event_num(rates.begin(), rates.end());

  std::mt19937_64 rng;
  rng.seed(p.get_rng_seed());
  return event_num(rng);
}
