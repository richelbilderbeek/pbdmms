#include "kewe_gausser.h"

#include <cassert>

#include "kewe_helper.h"

kewe::gausser::gausser(
  const double sd,
  const double tolerance
)
  : m_sd{sd}, m_tolerance{tolerance}
{
  assert(sd >= 0.0);
  assert(m_tolerance > 0.0);
  assert(m_tolerance < 1.0);
}

double kewe::gausser::operator()(const double x) const noexcept
{
  return gauss(x, m_sd);
}
