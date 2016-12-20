#include <iostream>
#include <boost/algorithm/string/split.hpp>
#include <boost/test/unit_test.hpp>

#include "kewe_individual.h"
#include "kewe_simulation.h"
#include "kewe_SES.h"
#include "kewe_parameters.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

using namespace kewe;

BOOST_AUTO_TEST_CASE(test_kewe_default_constructed_individuals_are_identical)
{
  const parameters p;
  const indiv a(p);
  const indiv b(p);
  BOOST_CHECK(a == a);
  BOOST_CHECK(b == b);
  BOOST_CHECK(a == b);
}

BOOST_AUTO_TEST_CASE(test_kewe_init_does_something_magical)
{
  const parameters p;
  indiv a(p);
  const indiv b(p);

  std::mt19937 gen(p.m_sim_parameters.seed);

  a.init(p, gen);
  BOOST_CHECK(a != b);

}

BOOST_AUTO_TEST_CASE(test_kewe_initial_individuals_are_different)
{
  parameters p;

  std::mt19937 gen(p.m_sim_parameters.seed);

  individuals pop = create_initial_population(p, gen);
  assert(pop.size() >= 3);
  const indiv a = pop[1];
  const indiv b = pop[2];
  BOOST_CHECK(a == a);
  BOOST_CHECK(b == b);
  BOOST_CHECK(a != b);
}

BOOST_AUTO_TEST_CASE(test_kewe_create_offsping_should_give_offspring_different_from_parents)
{
  const parameters p;
  const indiv a(p);
  const indiv b(a);
  assert(a == b);
  indiv kid(a);
  assert(kid == a);
  assert(kid == b);

  std::mt19937 gen(p.m_sim_parameters.seed);

  kid.birth(a, b, p, gen);
  BOOST_CHECK(kid != a);
  BOOST_CHECK(kid != b);
}

/*BOOST_AUTO_TEST_CASE(test_kewe_breaks_in_gui_should_break_here_too)
{
  #define FIX_ISSUE_81
  #ifdef FIX_ISSUE_81
  kewe::parameters p;
  p.sim_parameters.endtime = 1000;
  p.sim_parameters.popsize = 200;
  p.sim_parameters.sv = 0.1;
  simulation s(p);
  s.run();
  #endif
}*/
