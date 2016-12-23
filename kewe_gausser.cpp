#include "kewe_gausser.h"

#include <cassert>
#include <cmath>

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

double kewe::gausser::get_precise(const double x) const noexcept
{
  return std::exp(-(x*x) / (2.0*m_sd*m_sd));

}

double kewe::gausser::get_fast(const double x) const noexcept
{
  return get_precise(x);
}

double kewe::gausser::operator()(const double x) const noexcept
{
  return get_fast(x);
}

