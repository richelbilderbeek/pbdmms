#ifndef SADO_STD_DEVS_H
#define SADO_STD_DEVS_H

namespace sado {

/// corrected sample standard deviation of the traits p, q and x
class std_devs
{
public:
  /// corrected sample standard deviation of p
  double m_p;

  /// corrected sample standard deviation of q
  double m_q;

  /// corrected sample standard deviation of x
  double m_x;
};

} //~namespace sado

#endif // SADO_STD_DEVS_H
