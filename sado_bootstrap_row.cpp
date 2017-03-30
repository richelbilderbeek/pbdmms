#include "sado_bootstrap_row.h"

#include <cassert>
#include <iostream>

sado::bootstrap_row::bootstrap_row(
  const int ntips,
  const double b,
  const double mu_1,
  const double lambda_1,
  const double mu_2,
  const double loglik,
  const int df,
  const int conv,
  const double exp_durspec,
  const double median_durspec
) : m_ntips{ntips},
    m_b{b},
    m_mu_1{mu_1},
    m_lambda_1{lambda_1},
    m_mu_2{mu_2},
    m_loglik{loglik},
    m_df{df},
    m_conv{conv},
    m_exp_durspec{exp_durspec},
    m_median_durspec{median_durspec}
{
  assert(m_ntips >= 0);
  assert(m_b >= 0.0);
  assert(m_mu_1 >= 0.0);
  assert(m_lambda_1 >= 0.0);
  assert(m_mu_2 >= 0.0);
  assert(m_df > 0);
  assert(m_conv >= 0);
  assert(m_exp_durspec >= 0.0);
  assert(m_median_durspec >= 0.0);
}

std::ostream& sado::operator<<(std::ostream& os, const bootstrap_row& b) noexcept
{
  os
    << b.m_ntips << ','
    << b.m_b << ','
    << b.m_mu_1 << ','
    << b.m_lambda_1 << ','
    << b.m_mu_2 << ','
    << b.m_loglik << ','
    << b.m_df << ','
    << b.m_conv << ','
    << b.m_exp_durspec << ','
    << b.m_median_durspec
  ;
  return os;
}
