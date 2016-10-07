#include "jobo_parameters.h"
#include <iostream>
#include <stdexcept>
jobo_parameters::jobo_parameters(
  const int n_loci
) : m_n_loci{n_loci}
{
  if (n_loci < 0)
  {
    throw std::invalid_argument("n_loci must be positive");
  }



}
