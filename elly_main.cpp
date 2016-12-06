#include <cassert>
#include <iostream>
#include "elly_parameters.h"
#include "elly_rates.h"
#include <vector>

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




int main()
{
  try
  {
    using namespace elly;
    const rates r = calculate_rates(
      create_parameters_set1(),
      10,
      10,
      10
    );
    std::cout << r.mclad << '\n';

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

