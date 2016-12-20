#include "cine_simulation.h"
#include <numeric>		//needed for accumulate


// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_plot_set_and_get_should_be_symmetrical)
{

  const int x = 3;
  const int y = 5;

  plot patch(y, x);
  //patch.setPosition(x, y);

  BOOST_CHECK(patch.xposition() == x);
  BOOST_CHECK_EQUAL(patch.xposition(), x);
  BOOST_CHECK_EQUAL(patch.yposition(), y);
}


BOOST_AUTO_TEST_CASE(created_landscape)
{

  const int x = 3;
  const int y = 5;

  landscape patch = create_landscape(x, y);

  BOOST_CHECK(patch.size() == 3);
  BOOST_CHECK(patch[0].size() == 5);
  BOOST_CHECK_EQUAL(patch[2][1].xposition(), 2);
  BOOST_CHECK_EQUAL(patch[0][2].yposition(), 2);
}

BOOST_AUTO_TEST_CASE(movement_m)
{

  individual i;

  const int x = 1;
  const int y = 5;

  landscape patch = create_landscape(x, y);

  movement(i, patch);

  BOOST_CHECK(i.xposition() == 0);
  BOOST_CHECK(i.yposition() < 5);
  BOOST_CHECK(i.xposition() >= 0);
  BOOST_CHECK(i.yposition() >= 0);
}

BOOST_AUTO_TEST_CASE(calc_totalfood)
{
   population test(100);

for( int i = 0; i < 100; ++i){
 if(i < 33)
     test[i].food_uptake(0.75);
else
     test[i].food_uptake(0.1);

}

   std::vector<double>fitnesses_test = calculate_fitnesses_from_food(test);
    double sum = accumulate(fitnesses_test.begin(), fitnesses_test.end(), 0.00);
   BOOST_CHECK(fitnesses_test.size() == test.size());
   BOOST_CHECK(fitnesses_test.size() == 100);
   BOOST_CHECK(sum < 1.001);
   //BOOST_CHECK(fitnesses_test[99] == 0.1);

}


#pragma GCC diagnostic pop
