#include "sado_gausser.h"
#include <cassert>

sado::gausser::gausser(
  const double sd,
  const gausser_implementation gi
) : m_impl{create_gausser(gi, sd)}
{

}

