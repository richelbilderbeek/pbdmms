#include "sado_gausser.h"
#include <cassert>

sado::gausser::gausser(const double sd, const gausser_implementation gi)
    : m_impl{create_gausser(gi, sd)}
{
}

bool sado::operator==(const gausser& lhs, const gausser& rhs) noexcept
{
  return lhs.sd() == rhs.sd();
}
