#ifndef SADO_STD_DEVS_H
#define SADO_STD_DEVS_H

#include <iosfwd>

namespace sado {

/// corrected sample standard deviation of the traits p, q and x
class std_devs
{
public:
  std_devs(const double p, const double q, const double x)
    : m_p{p}, m_q{q}, m_x{x}
  {

  }
  /// corrected sample standard deviation of p
  double m_p;

  /// corrected sample standard deviation of q
  double m_q;

  /// corrected sample standard deviation of x
  double m_x;
};

std::ostream& operator<<(std::ostream &os, const std_devs& s) noexcept;

} //~namespace sado

#endif // SADO_STD_DEVS_H
