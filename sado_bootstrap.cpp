#include "sado_bootstrap.h"

#include <cassert>
#include <iostream>

sado::bootstrap::bootstrap(const likelihood& max_likelihood, const int n)
  : m_likelihoods{do_bootstrap(max_likelihood, n)}
{

}

std::vector<sado::likelihood> sado::do_bootstrap(
  const likelihood& max_likelihood,
  const int n)
{
  assert(n >= 0);
  assert(!"STUB");
  return std::vector<likelihood>(n, max_likelihood);
}

std::ostream& sado::operator<<(std::ostream& os, const bootstrap& b) noexcept
{
  for (const auto& likelihood: b.m_likelihoods)
  {
    os << likelihood << '\n';
  }
  return os;
}
