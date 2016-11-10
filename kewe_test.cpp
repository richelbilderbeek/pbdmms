#include <fstream>
#include <iostream>
#include <boost/test/unit_test.hpp>
#include "kewe_random.h"
// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

BOOST_AUTO_TEST_CASE(test_kewe)
{
  BOOST_CHECK(1 + 1 == 2);
  BOOST_CHECK_EQUAL(1 + 1, 2);
}

BOOST_AUTO_TEST_CASE(test_kewe_random)
{
  const int x{RandomNumber(1)};
  BOOST_CHECK_EQUAL(x, 0);


  BOOST_CHECK_THROW(RandomNumber(0), std::invalid_argument);
  BOOST_CHECK_THROW(RandomNumber(-42), std::invalid_argument);
}

#pragma GCC diagnostic pop



