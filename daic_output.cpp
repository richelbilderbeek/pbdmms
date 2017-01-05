#include "daic_output.h"
#include "daic_helper.h"
#include <string>
#include <cassert>
#include <fstream>

#include <iostream>

std::string daic::get_output_header() noexcept
{
  return R"(lambda_c","mu","K","gamma","lambda_a","loglik","df","conv")";
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
  output p;
  p.lambda_c = 0.767749320733306;
  p.mu = 1.77166095878803e-09;
  p.k = 3.00003880280079;
  p.gamma = 0.000750314431394791;
  p.lambda_a = 0.000101614571827502;
  p.loglik = -27.3882792747044;
  p.df = 5;
  p.conv = 0;
  return p;
}

daic::output daic::read_output_from_string(const std::string& s)
{
  std::vector<std::string> v = seperate_string(s, ',');
  assert(v.size() == 8);

  output p;
  p.lambda_c = std::stod(v[0]);
  p.mu = std::stod(v[1]);
  p.k = std::stod(v[2]);
  p.gamma = std::stod(v[3]);
  p.lambda_a = std::stod(v[4]);
  p.loglik = std::stod(v[5]);
  p.df = std::stoi(v[6]);
  p.conv = std::stoi(v[7]);

  return p;
}

daic::output daic::read_output_from_file(const std::string& filename )
{
  const std::vector<std::string> v = file_to_vector(filename);
  assert(v.size() >= 2);
  return read_output_from_string(v[1]);
}

bool daic::operator==(const output& lhs, const output& rhs) noexcept
{
  return
       lhs.conv == rhs.conv
    && lhs.df == rhs.df
    && lhs.gamma == rhs.gamma
    && lhs.k == rhs.k
    && lhs.lambda_a == rhs.lambda_a
    && lhs.lambda_c == rhs.lambda_c
    && lhs.loglik == rhs.loglik
    && lhs.mu == rhs.mu;
}

std::ostream& daic::operator<<(std::ostream& os, const output& p) noexcept
{
  os << p.lambda_c << '\t' << p.mu << '\t' << p.k << '\t' << p.gamma
     << '\t' << p.lambda_a << '\t' << p.loglik << '\t' << p.df << '\t' << p.conv;
  return os;
}
