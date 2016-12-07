#include <cassert>
#include <iostream>
#include "elly_parameters.h"
#include "elly_rates.h"
#include <vector>
#include <random>
#include <chrono>
#include <cstdlib>
#include <exception>


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
  r.set_mext( p.get_ext_rate_main() * mo);
  r.set_mimm( p.get_mig_rate_main() * nm * (1 - ni / p.get_carryingcap_is()));
  r.set_iext( p.get_ext_rate_is() * io);
  r.set_iimm( p.get_mig_rate_is() * ni * (1 - nm / p.get_carryingcap_main() ));
  r.set_bextm( p.get_ext_rate_main() * bo);
  r.set_bexti( p.get_ext_rate_is() * bo);
  r.set_bana( p.get_ana_rate() * bo);

  //if statements to avoid dividing by 0
  if(nm == 0){
      r.set_mclad(0.0);
      r.set_bcladm(0.0);
    } else{
      r.set_mclad(p.get_clado_rate_main() * (mo / nm) * (1 - nm / p.get_carryingcap_main()));
      r.set_bcladm( p.get_clado_rate_main() * (bo / nm ) * ( 1 - nm / p.get_carryingcap_main()));
    }

  if(ni == 0){
      r.set_iclad( 0);
      r.set_bcladi( 0);
    } else{
      r.set_iclad( p.get_clado_rate_is() * (io / ni) * (1 - ni / p.get_carryingcap_is()));
      r.set_bcladi( p.get_clado_rate_is() * (bo / ni) * ( 1 - ni / p.get_carryingcap_is()));
    }
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
    int mo = 500;
    int io = 0;
    int bo = 0;
    //setting initial conditions

    elly::parameters p = create_parameters_set1();

    const rates r = calculate_rates(
      p,
      mo,
      io,
      bo
    );
    std::cout << r.get_mclad() << '\n';

    time += draw_waiting_time( r, p);
    std::cout << time << '\n';

    int e = draw_event(r, p);
    std::cout<< e <<'\n';
    assert(e > 0);
    assert(e < 11);

    switch(e){
      case 0: //mainland cladogenesis
        break;
      case 1: //mainland extinction
        break;
      case 2: //mainland immigration
        break;
      case 3: //island extinction
        break;
      case 4: //island cladogenesis
        break;
      case 5: //island immigration
        break;
      case 6: //both habitats mainland extinction
        break;
      case 7: //both habitats island extinction
        break;
      case 8: //both habitats anagenesis
        break;
      case 9: //both habitats island cladogenesis
        break;
      case 10: //both habitats mainland cladogenesis
        break;
      }


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

