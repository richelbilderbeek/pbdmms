#include <fstream>
#include <iostream>
#include <boost/test/unit_test.hpp>
#include "kewe_individual.h"
// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

BOOST_AUTO_TEST_CASE(test_kewe_inidividual)
{
  BOOST_CHECK(1 + 1 == 2);
  BOOST_CHECK_EQUAL(1 + 1, 2);
}


#pragma GCC diagnostic pop




