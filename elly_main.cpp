#include <iostream>
#include "elly_parameters.h"

int main()
{
  try
  {
    using namespace elly;
    const double rate_clado_is{0.1};
    const double rate_clado_main{0.2};
    const double rate_ana{0.3};
    const double rate_ext_is{0.4};
    const double rate_ext_main{0.5};
    const double rate_mig_is{0.6};
    const double rate_mig_main{0.7};
    const int carryingcap_is{10};
    const int carryingcap_main{100};
    const unsigned int rng_seed{42};
    const parameters p(
      rate_clado_is,
      rate_clado_main,
      rate_ana,
      rate_ext_is,
      rate_ext_main,
      rate_mig_is,
      rate_mig_main,
      carryingcap_is,
      carryingcap_main,
      rng_seed
    );
    std::cout << "Created my first parameters\n"<< p << '\n';
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
