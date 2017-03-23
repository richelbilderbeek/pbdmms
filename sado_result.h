#ifndef SADO_RESULT_H
#define SADO_RESULT_H

#include "sado_histogram.h"
#include "sado_std_devs.h"

namespace sado
{

class result
{
public:
  result(
    const histogram& histp,
    const histogram& histq,
    const histogram& histx,
    const int pop_size,
    const double rhopq,
    const double rhoxp,
    const double rhoxq,
    const std_devs any_std_devs,
    //const double sp,
    //const double sq,
    //const double sx,
    const int t
  );

  /// Female preference histogram
  histogram m_histp;

  /// Male sexual trait histogram
  histogram m_histq;

  /// Ecological trait histogram
  histogram m_histx;

  /// Number of individuals
  int m_pop_size;

  double m_rhopq;
  double m_rhoxp;
  double m_rhoxq;

  /// corrected sample standard deviations of p, q and x;
  std_devs m_std_devs;
  //double m_sp;
  /// corrected sample standard deviation of q
  //double m_sq;
  /// corrected sample standard deviation of x
  //double m_sx;

  ///Time, in generation, first generation has m_t of zero
  int m_t;
};

std::ostream& operator<<(std::ostream& os, const result& r) noexcept;

} //~namespace sado

#endif // SADO_RESULT_H
