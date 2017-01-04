#include "sado_result.h"

#include <iostream>

std::ostream &sado::operator<<(std::ostream &os, const result &r) noexcept
{
  os << r.m_t << ',' << r.m_pop_size << ',' << r.m_rhoxp << ',' << r.m_rhoxq
     << ',' << r.m_rhopq << ',' << r.m_sx << ',' << r.m_sp << ',' << r.m_sq
     << ',' << r.m_histx << ',' << r.m_histp << ',' << r.m_histq;
  return os;
}
