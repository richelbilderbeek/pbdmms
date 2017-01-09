#include "daic_output.h"
#include "daic_helper.h"
#include <string>
#include <fstream>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace daic;

BOOST_AUTO_TEST_CASE(daic_output_construction_and_getters_must_match)
{
  const double lambda_c{1.1};
  const double mu{2.2};
  const double k{3.3};
  const double gamma{4.4};
  const double lambda_a{5.5};
  const double loglik{6.6};
  const int df{7};
  const bool conv{true};
  const output p(lambda_c, mu, k, gamma, lambda_a, loglik, df, conv);
  BOOST_CHECK_EQUAL(p.get_lambda_c(), lambda_c);
  BOOST_CHECK_EQUAL(p.get_mu(), mu);
  BOOST_CHECK_EQUAL(p.get_k(), k);
  BOOST_CHECK_EQUAL(p.get_gamma(), gamma);
  BOOST_CHECK_EQUAL(p.get_lambda_a(), lambda_a);
  BOOST_CHECK_EQUAL(p.get_loglik(), loglik);
  BOOST_CHECK_EQUAL(p.get_df(), df);
  BOOST_CHECK_EQUAL(p.get_conv(), conv);
}

BOOST_AUTO_TEST_CASE(daic_read_output_from_string)
{
  const std::string s{get_test_output_line()};
  const output p{read_output_from_string(s)};
  BOOST_CHECK_EQUAL(p.get_conv(), false);
  BOOST_CHECK_EQUAL(p.get_df(), 5);
  BOOST_CHECK_CLOSE(p.get_gamma(), 0.000750314431394791, 0.0001);
  BOOST_CHECK_CLOSE(p.get_k(), 3.00003880280079, 0.0001);
  BOOST_CHECK_CLOSE(p.get_lambda_a(), 0.000101614571827502, 0.0001);
  BOOST_CHECK_CLOSE(p.get_lambda_c(), 0.767749320733306, 0.0001);
  BOOST_CHECK_CLOSE(p.get_loglik(), -27.3882792747044, 0.0001);
  BOOST_CHECK_CLOSE(p.get_mu(), 1.77166095878803e-09, 0.0001);
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

