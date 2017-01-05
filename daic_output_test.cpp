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
  BOOST_CHECK_CLOSE(p.gamma, 0.000750314431394791, 0.0001);
  BOOST_CHECK_CLOSE(p.k, 3.00003880280079, 0.0001);
  BOOST_CHECK_CLOSE(p.lambda_a, 0.000101614571827502, 0.0001);
  BOOST_CHECK_CLOSE(p.lambda_c, 0.767749320733306, 0.0001);
  BOOST_CHECK_CLOSE(p.loglik, -27.3882792747044, 0.0001);
  BOOST_CHECK_CLOSE(p.mu, 1.77166095878803e-09, 0.0001);
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

