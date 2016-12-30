#ifndef SADO_RESULT_H
#define SADO_RESULT_H

#include "sado_histogram.h"

namespace sado {

class result {
public:
  histogram m_histx;
  histogram m_histp;
  histogram m_histq;

  // Timepoints that have been measured
  int m_t;

  int m_pop_size;

  double m_rhopq;
  double m_rhoxp;
  double m_rhoxq;
  double m_sx;
  double m_sp;
  double m_sq;
};

std::ostream &operator<<(std::ostream &os, const result &r) noexcept;

} //~namespace sado

#endif // SADO_RESULT_H
