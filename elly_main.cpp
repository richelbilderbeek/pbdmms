#include <cassert>
#include <iostream>
#include "elly_parameters.h"
#include "elly_rates.h"
#include <vector>
#include <random>
#include <chrono>
#include <cstdlib>


//Calculates the rates
//mo number of mainland-only species
elly::rates calculate_rates(const elly::parameters& p, int mo , int io , int bo)
{
  elly::rates r;
  mo = static_cast<double>(mo);
  io = static_cast<double>(io);
  bo = static_cast<double>(bo);

  int nm   = mo + bo;
  int ni   = io + bo;
  r.mext   = p.get_ext_rate_main() * mo;
  r.mclad  = p.get_clado_rate_main() * (mo / nm) * (1 - nm / p.get_carryingcap_main());
  r.mimm   = p.get_mig_rate_main() * nm * (1 - ni / p.get_carryingcap_is());
  r.iext   = p.get_ext_rate_is() * io;
  r.iclad  = p.get_clado_rate_is() * (io / ni) * (1 - ni / p.get_carryingcap_is());
  r.iimm   = p.get_mig_rate_is() * ni * (1 - nm / p.get_carryingcap_main() );
  r.bextm  = p.get_ext_rate_main() * bo;
  r.bexti  = p.get_ext_rate_is() * bo;
  r.bana   = p.get_ana_rate() * bo;
  r.bcladi = p.get_clado_rate_is() * (bo / ni) * ( 1 - ni / p.get_carryingcap_is());
  r.bcladm = p.get_clado_rate_main() * (bo / nm ) * ( 1 - nm / p.get_carryingcap_main());

  return r;
}

//draws random waiting time until next event
double draw_waiting_time(const elly::rates& r, const elly::parameters& p)
{
  std::mt19937_64 rng;
  rng.seed(p.get_rng_seed());

  std::exponential_distribution<double> waiting_time(calc_sumrates(r));
  double wt = waiting_time(rng);
  return wt;
}

//draws random event that takes place
int draw_event(const elly::rates& r , const elly::parameters& p)
{
  std::vector<double> rates = elly::to_ratesvector(r);
  std::discrete_distribution<int> event_num(rates.begin(), rates.end());

  std::mt19937_64 rng;
  rng.seed(p.get_rng_seed());
  return event_num(rng);
}

int main()
{
  try
  {
    using namespace elly;

    double time = 0.0;

    elly::parameters p = create_parameters_set1();

    const rates r = calculate_rates(
      p,
      500,
      20,
      10
    );
    std::cout << r.mclad << '\n';

    time += draw_waiting_time( r, p);
    std::cout << time << '\n';

    int e = draw_event(r, p);
    //Insert switch statement



  }
  catch (std::exception& e)
  {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }
  catch (...)
  {
    std::cerr << "Error: Unknown\n";
    return 1;
  }
}

