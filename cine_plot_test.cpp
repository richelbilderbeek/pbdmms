#include "cine_plot.h"


// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_CASE(test_plot_set_and_get_should_be_symmetrical)
{

  const int x = 3;
  const int y = 5;

  plot patch(y, x);

  BOOST_CHECK(patch.xposition() == x);
  BOOST_CHECK_EQUAL(patch.xposition(), x);
  BOOST_CHECK_EQUAL(patch.yposition(), y);
}

BOOST_AUTO_TEST_CASE(test_growing_grass)
{
    plot patch(3,5);
    patch.grass_consumption();
    patch.let_grass_grow();


BOOST_CHECK(patch.grass_height() == 1);
for (int i = 0; i < 10; ++i){
BOOST_CHECK(patch.grass_height() <=1);
}
}

#pragma GCC diagnostic pop
