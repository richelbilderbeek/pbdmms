#include "daic_output.h"
#include "daic_helper.h"
#include <string>
#include <cassert>
#include <fstream>

#include <iostream>

std::string daic::get_output_header() noexcept
{
  //Just copied the whitespace as such
  return "lambda_c           mu       K      gamma     lambda_a    loglik df conv";
}

std::string daic::get_test_output_line() noexcept
{
  //Just copied the whitespace as such
  return "1.137017 3.548139e-07 3.34202 0.00100163 1.533841e-05 -39.75229  5    0";
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
  p.lambda_c = 1.137017;
  p.mu = 3.548139e-07;
  p.k = 3.34202;
  p.gamma = 0.00100163;
  p.lambda_a = 1.533841e-05;
  p.loglik = -39.75229;
  p.df = 5;
  p.conv = 0;
  return p;
}

daic::output daic::read_output_from_string(const std::string& s)
{
  std::vector<std::string> v = seperate_string(s, ' ');
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
  std::vector<std::string> v = file_to_vector(filename);
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

bool daic::operator==(const output& lhs, const output& rhs) noexcept
{
  return lhs.conv != rhs.conv; //STUB
}

std::ostream& daic::operator<<(std::ostream& os, const output& p) noexcept
{
  os << p.conv << "STUB";
  return os;
}
