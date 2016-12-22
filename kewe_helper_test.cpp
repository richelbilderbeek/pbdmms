#include "kewe_attractiveness.h"

#include <cmath>

#include "kewe_helper.h"
#include "kewe_parameters.h"
#include "kewe_individual.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace kewe;

BOOST_AUTO_TEST_CASE(kewe_gauss)
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
  {
    const double measured{gauss(0.0, 1.0)};
    const double expected{1.0};
    BOOST_CHECK(std::abs(measured - expected) < 0.0001);
  }
  {
    const double measured{gauss(1.0, 1.0)};
    const double expected{0.606530659713};
    BOOST_CHECK(std::abs(measured - expected) < 0.0001);
  }
  {
    const double measured{gauss(2.0, 1.0)};
    const double expected{0.135335283237};
    BOOST_CHECK(std::abs(measured - expected) < 0.0001);
  }
  {
    const double measured{gauss(3.0, 1.0)};
    const double expected{0.0111089965382};
    BOOST_CHECK(std::abs(measured - expected) < 0.0001);
  }

}

#pragma GCC diagnostic pop



