#include "kewe_gausser.h"

#include "kewe_helper.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace kewe;

BOOST_AUTO_TEST_CASE(kewe_gauss_precise)
{
  /// std::exp(-(x*x) / (2.0*m_sd*m_sd))
  /// std::exp(-(4*4) / (2.0))
  /// Examples are:
  /// +-----+-----------------------------+
  /// | x   | gauss(x, 1.0)               |
  /// +-----+-----------------------------+
  /// | 0.0 | e(-0.0  ) = 1.0             |
  /// | 1.0 | e(-0.5  ) = 0.606530659713  |
  /// | 2.0 | e(-2.0  ) = 0.135335283237  |
  /// | 3.0 | e(-4.5  ) = 0.0111089965382 |
  /// | 3.5 | e(-6.125) = 0.002187491     |
  /// | 4.0 | e(-8.0  ) = 0.0003354626279 |
  /// +-----+-----------------------------+
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
  {
    const double measured{g.get_precise(3.5)};
    const double expected{0.002187491};
    BOOST_CHECK_CLOSE(measured, expected, 0.0001);
  }
  {
    const double measured{g.get_precise(4.0)};
    const double expected{0.0003354626279};
    BOOST_CHECK_CLOSE(measured, expected, 0.0001);
  }

}

BOOST_AUTO_TEST_CASE(kewe_gausser)
{
  const double sd{1.0};
  const gausser g(sd);
  //Must be in range of gausser, which is [-4.0,4.0]
  for (double x = -3.9; x < 3.9; x+=0.001)
  {
    const double expected{gauss(x, sd)};
    const double measured{g(x)};
    BOOST_CHECK_CLOSE(measured, expected, 0.000001);
  }
}


#pragma GCC diagnostic pop




