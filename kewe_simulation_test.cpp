#include <iostream>
#include <boost/algorithm/string/split.hpp>

#include "kewe_individual.h"
#include "kewe_simulation.h"
#include "kewe_ses.h"
#include "kewe_parameters.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace kewe;

BOOST_AUTO_TEST_CASE(test_kewe_default_constructed_individuals_are_identical)
{
  const simulation_parameters p;
  const individual a(p);
  const individual b(p);
  BOOST_CHECK(a == a);
  BOOST_CHECK(b == b);
  BOOST_CHECK(a == b);
}

BOOST_AUTO_TEST_CASE(test_kewe_init_does_something_magical)
{
  const simulation_parameters p;
  individual a(p);
  const individual b(p);

  std::mt19937 gen(p.seed);

  a.init(p, gen);
  BOOST_CHECK(a != b);

}

BOOST_AUTO_TEST_CASE(test_kewe_initial_individuals_are_different)
{
  simulation_parameters p;

  std::mt19937 gen(p.seed);

  individuals pop = create_initial_population(p, gen);
  assert(pop.size() >= 3);
  const individual a = pop[1];
  const individual b = pop[2];
  BOOST_CHECK(a == a);
  BOOST_CHECK(b == b);
  BOOST_CHECK(a != b);
}

BOOST_AUTO_TEST_CASE(test_kewe_create_offsping_should_give_offspring_different_from_parents)
{
  const simulation_parameters p;
  const individual a(p);
  const individual b(a);
  assert(a == b);
  individual kid(a);
  assert(kid == a);
  assert(kid == b);

  std::mt19937 gen(p.seed);

  kid.birth(a, b, p, gen);
  BOOST_CHECK(kid != a);
  BOOST_CHECK(kid != b);
}

/*BOOST_AUTO_TEST_CASE(test_kewe_breaks_in_gui_should_break_here_too)
{
  #define FIX_ISSUE_81
  #ifdef FIX_ISSUE_81
  simulation_parameters p;
  p.sim_parameters.endtime = 1000;
  p.sim_parameters.popsize = 200;
  p.sim_parameters.sv = 0.1;
  simulation s(p);
  s.run();
  #endif
}*/
