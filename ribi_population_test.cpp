#include "distancer_population.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_tally_sils)
{
  population p;
  const auto m = tally_sils(p);
  BOOST_CHECK_EQUAL(m.size(), 0);
}

