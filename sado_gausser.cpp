#include "sado_gausser.h"
#include <cassert>

#include "gausser.h"
#include "raw_gausser.h"

namespace sado {

class gausser_impl
{
public:
  virtual ~gausser_impl() {}

  ///Get the standard deviation of the gaussian
  virtual double sd() const noexcept = 0;

  ///Get the density at the gaussion at x. Will be 1.0 for x equals 0.0
  virtual double operator()(const double x) const noexcept = 0;
};

class gausser_impl_lut : public gausser_impl
{
public:
  gausser_impl_lut(const double sd) : m_g(sd) {}

  ///Get the standard deviation of the gaussian
  double sd() const noexcept override { return m_g.sd(); }

  ///Get the density at the gaussion at x. Will be 1.0 for x equals 0.0
  double operator()(const double x) const noexcept override { return m_g(x); }

private:

  const ribi::gausser m_g;
};

class gausser_impl_raw : public gausser_impl
{
public:
  gausser_impl_raw(const double sd) : m_g(sd) {}

  ///Get the standard deviation of the gaussian
  double sd() const noexcept override { return m_g.sd(); }

  ///Get the density at the gaussion at x. Will be 1.0 for x equals 0.0
  double operator()(const double x) const noexcept override { return m_g(x); }

private:

  const ribi::raw_gausser m_g;
};

gausser_impl * create_gausser(const gausser_implementation gi, const double sd)
{
  if (gi == gausser_implementation::raw)
  {
    return new gausser_impl_raw(sd);
  }
  assert(gi == gausser_implementation::lut);
  return new gausser_impl_lut(sd);
}


} //~namespace sado


sado::gausser::gausser(
  const double sd,
  const gausser_implementation gi
) : m_impl{create_gausser(gi, sd)}
{

}

double sado::gausser::sd() const noexcept
{
  assert(m_impl);
  return m_impl->sd();
}

double sado::gausser::operator()(const double x) const noexcept
{
  assert(m_impl);
  return m_impl->operator ()(x);
}
