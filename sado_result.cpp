#include "sado_result.h"

#include <iostream>

sado::result::result( //!OCLINT too many parameters indeed
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
) : m_histp{histp},
    m_histq{histq},
    m_histx{histx},
    m_pop_size{pop_size},
    m_rhopq{rhopq},
    m_rhoxp{rhoxp},
    m_rhoxq{rhoxq},
    m_std_devs{any_std_devs},
    //m_sp{sp},
    //m_sq{sq},
    //m_sx{sx},
    m_t{t}

{
}

std::ostream& sado::operator<<(std::ostream& os, const result& r) noexcept
{
  os << r.m_t << ',' << r.m_pop_size << ',' << r.m_rhoxp << ',' << r.m_rhoxq
     << ',' << r.m_rhopq << ','
     << r.m_std_devs
     //<< r.m_sx << ',' << r.m_sp << ',' << r.m_sq
     << ',' << r.m_histx << ',' << r.m_histp << ',' << r.m_histq;
  return os;
}
