#include "cine_simulation.h"
#include <numeric>		//needed for accumulate
#include <vector>		// for vector related commands


using namespace std;

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>



///Tests function predation_simulation
BOOST_AUTO_TEST_CASE(predation_functest)
{


    population t_h(1);
    population t_p(1);
    t_h[0].setPosition(1, 1);
    t_p[0].setPosition(1, 1);

    landscape t_Plots = create_landscape(3, 3);//landscape is created

    for_each(t_Plots, [](plot& p) { p.setRisk(0.5); } );//risk is assigned

    population copy_th =  t_h;
    population copy_tp =  t_p;
    //
    int pred_success = 0;
    for(int i = 0; i < 100; ++i){
        predation_simulation(t_h, t_p, t_Plots);
        if(t_h.size() == 0){
            ++pred_success;
            t_h = copy_th;
            t_p = copy_tp;
        }
    }

    predation_simulation(t_h, t_p, t_Plots);

    BOOST_CHECK(t_h.size() == 0 || t_p[0].return_food() == 0);
    BOOST_CHECK(pred_success < 75);
    BOOST_CHECK(pred_success > 25);

}








BOOST_AUTO_TEST_CASE(test_new_generation)
{
    std::vector<double> t_fitnesses_prey;
            population t_h(4);
    for (int i = 0; i < 4; ++i){
        if(i < 3){
            t_h[i].setPosition(2,2);
            t_fitnesses_prey.push_back(0);
        }
        else {
            t_h[i].setPosition(4,4);
            t_fitnesses_prey.push_back(1);
        }
    }
    population copy_th = t_h;
    new_generation(t_h, t_fitnesses_prey, 4);

    BOOST_CHECK_EQUAL(t_h.size(), copy_th.size());
    BOOST_CHECK(copy_th[3].xposition() == 4);
    BOOST_CHECK(t_h[2].xposition() == 4);
    //BOOST_CHECK(t_h != copy_th);


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

  random_movement(i, patch);

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
     test[i].food_update(0.75);
else
     test[i].food_update(0.1);

}
int ANN_energycost = -0.15;
   std::vector<double>fitnesses_test = calculate_fitnesses_from_food(test, ANN_energycost);
    double sum = accumulate(fitnesses_test.begin(), fitnesses_test.end(), 0.00);
   BOOST_CHECK(fitnesses_test.size() == test.size());
   BOOST_CHECK(fitnesses_test.size() == 100);
   BOOST_CHECK(sum < 1.001);
   //BOOST_CHECK(fitnesses_test[99] == 0.1);

}

/*
BOOST_AUTO_TEST_CASE(test_entirety)
{
    const int   generations = 3;
    const int   ncols =  10;
    const int   nrows = 10;
    const int   prey_pop = 25;
    const int   predator_pop = 25;
    const float prob_mutation_to_0 = 0.05;
    const float prob_mutation_to_rd = 0.025;
    const int   timesteps = 10;
do_simulation(generations,
              ncols, nrows, prey_pop,
              predator_pop, prob_mutation_to_0,
              prob_mutation_to_rd, timesteps);

}

*/

#pragma GCC diagnostic pop
