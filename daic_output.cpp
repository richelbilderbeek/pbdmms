#include "daic_output.h"
#include "daic_helper.h"
#include <string>
#include <cassert>
#include <fstream>

#include <iostream>

daic::output::output(
  const double lambda_c,
  const double mu,
  const double k,
  const double gamma,
  const double lambda_a,
  const double loglik,
  const int df,
  const bool conv
)
  : m_lambda_c{lambda_c},
    m_mu{mu},
    m_k{k},
    m_gamma{gamma},
    m_lambda_a{lambda_a},
    m_loglik{loglik},
    m_df{df},
    m_conv{conv}
{

}

std::string daic::get_output_header() noexcept
{
  return R"("lambda_c","mu","K","gamma","lambda_a","loglik","df","conv")";
}

std::string daic::get_test_output_line() noexcept
{
  return "0.767749320733306,1.77166095878803e-09,3.00003880280079,"
    "0.000750314431394791,0.000101614571827502,-27.3882792747044,5,0";

}

std::vector<std::string> daic::get_test_output_lines() noexcept
{
  return
  {
    get_output_header(),
    get_test_output_line()
  };
}

daic::output daic::get_test_output() noexcept
{
  const double lambda_c = 0.767749320733306;
  const double mu = 1.77166095878803e-09;
  const double k = 3.00003880280079;
  const double gamma = 0.000750314431394791;
  const double lambda_a = 0.000101614571827502;
  const double loglik = -27.3882792747044;
  const int df = 5;
  const bool conv = 0;
  return output(
    lambda_c,
    mu,
    k,
    gamma,
    lambda_a,
    loglik,
    df,
    conv
  );
}

bool daic::is_empty(const output& p) noexcept
{
  return p == output();
}

bool daic::is_null(const output& p) noexcept
{
  const output q;
  return p == q;
}

daic::output daic::read_output_from_string(const std::string& s)
{
  std::vector<std::string> v = seperate_string(s, ',');
  assert(v.size() == 8);

  return output(
    std::stod(v[0]),
    std::stod(v[1]),
    std::stod(v[2]),
    std::stod(v[3]),
    std::stod(v[4]),
    std::stod(v[5]),
    std::stoi(v[6]),
    std::stoi(v[7])
  );
}

daic::output daic::read_output_from_file(const std::string& filename )
{
  assert(is_regular_file(filename));
  const std::vector<std::string> v = file_to_vector(filename);
  assert(v.size() >= 2);
  return read_output_from_string(v[1]);
}

bool daic::operator==(const output& lhs, const output& rhs) noexcept
{
  return
       lhs.m_conv == rhs.m_conv
    && lhs.m_df == rhs.m_df
    && lhs.m_gamma == rhs.m_gamma
    && lhs.m_k == rhs.m_k
    && lhs.m_lambda_a == rhs.m_lambda_a
    && lhs.m_lambda_c == rhs.m_lambda_c
    && lhs.m_loglik == rhs.m_loglik
    && lhs.m_mu == rhs.m_mu;
}

std::ostream& daic::operator<<(std::ostream& os, const output& p) noexcept
{
  os
    << p.m_lambda_c << '\t'
    << p.m_mu << '\t'
    << p.m_k << '\t'
    << p.m_gamma  << '\t'
    << p.m_lambda_a << '\t'
    << p.m_loglik << '\t'
    << p.m_df << '\t'
    << p.m_conv;
  return os;
}

std::istream& daic::operator>>(std::istream& is, output& p)
{
  is
    >> p.m_lambda_c
    >> p.m_mu
    >> p.m_k
    >> p.m_gamma
    >> p.m_lambda_a
    >> p.m_loglik
    >> p.m_df
    >> p.m_conv
  ;
  return is;
}

