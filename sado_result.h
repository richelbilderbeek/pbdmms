#ifndef SADO_RESULT_H
#define SADO_RESULT_H

#include "sado_histogram.h"

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
    const double sp,
    const double sq,
    const double sx,
    const int t
  );
  histogram m_histp;
  histogram m_histq;
  histogram m_histx;
  int m_pop_size;
  double m_rhopq;
  double m_rhoxp;
  double m_rhoxq;
  double m_sp;
  double m_sq;
  double m_sx;
  ///Time, in generation, first generation has m_t of zero
  int m_t;
};

std::ostream &operator<<(std::ostream &os, const result& r) noexcept;

} //~namespace sado

#endif // SADO_RESULT_H
