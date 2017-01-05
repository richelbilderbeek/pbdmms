#include "daic_output.h"
#include "daic_helper.h"
#include <string>
#include <fstream>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace daic;

BOOST_AUTO_TEST_CASE(daic_read_output_from_string)
{
  const std::string s{get_test_output_line()};
  const output p{read_output_from_string(s)};
  BOOST_CHECK_EQUAL(p.conv, false);
  BOOST_CHECK_EQUAL(p.df, 5);
  BOOST_CHECK_CLOSE(p.gamma, 0.00100163, 0.0001);
  BOOST_CHECK_CLOSE(p.k, 3.34202, 0.0001);
  BOOST_CHECK_CLOSE(p.lambda_a, 1.533841e-05, 0.0001);
  BOOST_CHECK_CLOSE(p.lambda_c, 1.137017, 0.0001);
  BOOST_CHECK_CLOSE(p.loglik, -39.75229, 0.0001);
  BOOST_CHECK_CLOSE(p.mu, 3.548139e-07, 0.0001);
}

BOOST_AUTO_TEST_CASE(daic_read_output_from_file)
{
  const std::string filename = "Test_file.txt";
  const std::vector<std::string> v = get_test_output_lines();
  {
    std::ofstream os(filename);
    for(const std::string& s: v)
    {
      os << s << '\n';
    }
  }
  const output t = read_output_from_file(filename);
  const output q = get_test_output();
  BOOST_CHECK_EQUAL(t, q);
}

#pragma GCC diagnostic pop

