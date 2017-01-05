#include "daic_input.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace daic;

BOOST_AUTO_TEST_CASE(daic_)
{
  BOOST_CHECK(1 == 2);
}


#pragma GCC diagnostic pop
