#include "cine_simulation.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_plot_set_and_get_should_be_symmetrical)
{
  plot p;
  const int x = 3;
  const int y = 5;
  p.setPosition(x, y);

  BOOST_CHECK(p.xposition() == x);
  BOOST_CHECK_EQUAL(p.xposition(), x);
  BOOST_CHECK_EQUAL(p.yposition(), y);
}

#pragma GCC diagnostic pop
