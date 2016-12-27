#include "sado_parameters.h"

sado::parameters::parameters(
    const int pop_size,
    const erasure e
  )
  : m_erasure{e},
    m_pop_size{pop_size}
{

}


