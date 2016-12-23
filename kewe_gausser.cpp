#include "kewe_gausser.h"

#include <cassert>
#include <cmath>

#include "kewe_helper.h"

kewe::gausser::gausser(const double sd)
  : m_sd{sd}, m_lut{create_lut(sd, 8000)}
{
  assert(sd >= 0.0);
}

std::vector<double> kewe::gausser::create_lut(const double sd, const int sz)
{
  std::vector<double> v;
  v.reserve(sz + 1);
  const double dx{8.0 / static_cast<double>(sz)};
  for (double x = -4.0; x < 4.0; x += dx)
  {
    v.push_back(gauss(x, sd));
  }
  return v;
}

double kewe::gausser::get_precise(const double x) const noexcept
{
  return std::exp(-(x*x) / (2.0*m_sd*m_sd));

}

double kewe::gausser::get_fast(const double x) const noexcept
{
  //const double min_x{-4.0};
  //const double max_x{4.0};

  //Fraction of range
  const double f{(x + 4.0) / 8.0};
  const int i{
    static_cast<int>(
      f * static_cast<double>(m_lut.size())
    )
  };
  if (i < 0) return m_lut[0];
  if (i > static_cast<int>(m_lut.size())) return m_lut.back();
  return m_lut[i];
}

double kewe::gausser::operator()(const double x) const noexcept
{
  return get_fast(x);
}

