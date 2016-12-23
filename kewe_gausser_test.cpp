#include "kewe_gausser.h"

#include "kewe_helper.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace kewe;

BOOST_AUTO_TEST_CASE(kewe_gauss_precise)
{
  /// Examples are:
  /// +-----+---------------------------+
  /// | x   | gauss(x, 1.0)             |
  /// +-----+---------------------------+
  /// | 0.0 | e(-0.0) = 1.0             |
  /// | 1.0 | e(-0.5) = 0.606530659713  |
  /// | 2.0 | e(-2.0) = 0.135335283237  |
  /// | 3.0 | e(-4.5) = 0.0111089965382 |
  /// +-----+---------------------------+
  /// @param x distance from zero
  /// @param sigma standard deviation
  /// @return the density of a normal distribution
  const double sd{1.0};
  const gausser g(sd);
  {
    const double measured{g.get_precise(0.0)};
    const double expected{1.0};
    BOOST_CHECK_CLOSE(measured, expected, 0.0001);
  }
  {
    const double measured{g.get_precise(1.0)};
    const double expected{0.606530659713};
    BOOST_CHECK_CLOSE(measured, expected, 0.0001);
  }
  {
    const double measured{g.get_precise(2.0)};
    const double expected{0.135335283237};
    BOOST_CHECK_CLOSE(measured, expected, 0.0001);
  }
  {
    const double measured{g.get_precise(3.0)};
    const double expected{0.0111089965382};
    BOOST_CHECK_CLOSE(measured, expected, 0.0001);
  }

}

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




