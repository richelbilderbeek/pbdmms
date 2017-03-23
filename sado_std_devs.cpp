#include "sado_std_devs.h"

#include <iostream>

std::ostream& sado::operator<<(std::ostream& os, const std_devs& s) noexcept
{
  os << s.m_x << ',' << s.m_p << ',' << s.m_q;
  return os;
}
