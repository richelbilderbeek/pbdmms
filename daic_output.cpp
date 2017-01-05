#include "daic_output.h"

std::string daic::get_output_header() noexcept
{
  //Just copied the whitespace as such
  return "lambda_c           mu       K      gamma     lambda_a    loglik df conv";
}

std::string daic::get_test_output_line() noexcept
{
  //Just copied the whitespace as such
  return "1 1.137017 3.548139e-07 3.34202 0.00100163 1.533841e-05 -39.75229  5    0";
}

std::vector<std::string> daic::get_test_output_lines() noexcept
{
  return
  {
    get_output_header(),
    get_test_output_line()
  };
}

daic::output daic::read_output_from_file(const std::string& /* filename */)
{
  return {}; //STUB
}
