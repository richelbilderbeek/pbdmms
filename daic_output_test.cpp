#include "daic_output.h"

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
  BOOST_CHECK_CLOSE(p.gamma, 3.34202, 0.0001);
  //ELLY_TODO
}

BOOST_AUTO_TEST_CASE(daic_read_output_from_file)
{
  //ELLY_TODO
}

#pragma GCC diagnostic pop

