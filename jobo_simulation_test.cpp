#include "jobo_simulation.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_CASE(test_boost_dynamic_bitset)
{
  //jobo_simulation s;
  BOOST_CHECK(1 == 2);
}

#pragma GCC diagnostic pop
