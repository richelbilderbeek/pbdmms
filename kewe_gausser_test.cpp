#include "kewe_gausser.h"

#include "kewe_helper.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace kewe;

BOOST_AUTO_TEST_CASE(kewe_gausser)
{
  const double sd{1.0};
  const gausser g(sd);
  for (double x = -4.0; x < 4.0; x+=0.1)
  {
    const double expected{gauss(x, sd)};
    const double measured{g(x)};
    BOOST_CHECK_CLOSE(measured, expected, 0.001);
  }
}

#pragma GCC diagnostic pop




