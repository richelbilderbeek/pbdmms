#include "elly_parameters.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>


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
         const unsigned int rng_seed
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
        m_rng_seed{rng_seed}
 {

 }
