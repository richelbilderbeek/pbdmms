#include "sado_gausser.h"
#include <cassert>

//#include "gausser.h"
#include "raw_gausser.h"

namespace sado {

class gausser_impl
{
public:
  gausser_impl() {}
  virtual ~gausser_impl() {}

  ///Get the standard deviation of the gaussian
  virtual double sd() const noexcept = 0;

  ///Get the density at the gaussion at x. Will be 1.0 for x equals 0.0
  virtual double operator()(const double x) const noexcept = 0;
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

} //~namespace sado

sado::gausser::gausser(
  const double sd,
  const gausser_implementation /* gi */
) : m_impl{new gausser_impl_raw(sd)}
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
