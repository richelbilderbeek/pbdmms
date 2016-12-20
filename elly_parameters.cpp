#include "elly_parameters.h"

#include <cassert>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <cstdlib>


elly::parameters::parameters(
  const double rate_clado_main,
  const double rate_clado_is,
  const double rate_ana,
  const double rate_ext_is,
  const double rate_ext_main,
  const double rate_mig_is,
  const double rate_mig_main,
  const int carryingcap_is,
  const int carryingcap_main,
  const int rng_seed,
  const int main_init,
  const double crown_age
)
  : m_rate_clado_is{rate_clado_is},
    m_rate_clado_main{rate_clado_main},
    m_rate_ana{rate_ana},
    m_rate_ext_is{rate_ext_is},
    m_rate_ext_main{rate_ext_main},
    m_rate_mig_is{rate_mig_is},
    m_rate_mig_main{rate_mig_main},
    m_carryingcap_is{carryingcap_is},
    m_carryingcap_main{carryingcap_main},
    m_rng_seed{rng_seed},
    m_main_init{main_init},
    m_crown_age{crown_age}
{
  assert(m_rate_clado_is >= 0.0);
  assert(m_rate_clado_main >= 0.0);
  assert(m_rate_ana >= 0.0);
  assert(m_rate_ext_is >= 0.0);
  assert(m_rate_ext_main >= 0.0);
  assert(m_rate_mig_is >= 0.0);
  assert(m_rate_mig_main >= 0.0);
  assert(m_carryingcap_is > 0);
  assert(m_carryingcap_main > 0);
  assert(m_main_init > 0);
  assert(m_crown_age > 0.0);
}

elly::parameters elly::create_parameters_set1() noexcept
 {
     const double rate_clado_main{0.20};
     const double rate_clado_is{0.20};
     const double rate_ana{0.87};
     const double rate_ext_is{0.09};
     const double rate_ext_main{0.09};
     const double rate_mig_is{0.002};
     const double rate_mig_main{0.002};
     const int carryingcap_is{30};
     const int carryingcap_main{1100};
     //still needs to be determined if carrying capacity is right value
     const unsigned int rng_seed{117};
     const int main_init{500};
     const double crown_age{10.0};
     return parameters (
       rate_clado_main,
       rate_clado_is,
       rate_ana,
       rate_ext_is,
       rate_ext_main,
       rate_mig_is,
       rate_mig_main,
       carryingcap_is,
       carryingcap_main,
       rng_seed,
       main_init,
       crown_age
     );
 }

bool elly::operator==(const parameters& lhs, const parameters& rhs) noexcept
{
  return
    lhs.get_ana_rate() == rhs.get_ana_rate()
    && lhs.get_mig_rate_is() == rhs.get_mig_rate_is()
    && lhs.get_mig_rate_main() == rhs.get_mig_rate_main()
    && lhs.get_ext_rate_is() == rhs.get_ext_rate_is()
    && lhs.get_ext_rate_main() == rhs.get_ext_rate_main()
    && lhs.get_clado_rate_is() == rhs.get_clado_rate_is()
    && lhs.get_clado_rate_main() == rhs.get_clado_rate_main()
    && lhs.get_carryingcap_is() == rhs.get_carryingcap_is()
    && lhs.get_carryingcap_main() == rhs.get_carryingcap_main()
    && lhs.get_rng_seed() == rhs.get_rng_seed()
    && lhs.get_main_init() == rhs.get_main_init()
  ;
}


