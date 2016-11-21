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

BOOST_AUTO_TEST_CASE(test_kewe_default_constructed_individuals_are_identical)
{
  const kewe_parameters parameters;
  const indiv a(parameters);
  const indiv b(parameters);
  BOOST_CHECK(a == a);
  BOOST_CHECK(b == b);
  BOOST_CHECK(a == b);
}

BOOST_AUTO_TEST_CASE(test_kewe_initiatsdbdth)
{
  const kewe_parameters parameters;
  indiv a(parameters);
  const indiv b(parameters);
  a.init(parameters);
  BOOST_CHECK(a != b);

}

BOOST_AUTO_TEST_CASE(test_kewe_initial_individuals_are_different)
{
  kewe_parameters parameters;
  std::vector<indiv> pop = create_initial_population(parameters);
  assert(pop.size() >= 3);
  const indiv a = pop[1];
  const indiv b = pop[2];
  BOOST_CHECK(a == a);
  BOOST_CHECK(b == b);
  BOOST_CHECK(a != b);
}

BOOST_AUTO_TEST_CASE(test_kewe_create_offsping_should_give_offspring_different_from_parents)
{
  const kewe_parameters parameters;
  const indiv a(parameters);
  const indiv b(a);
  assert(a == b);
  indiv kid(a);
  assert(kid == a);
  assert(kid == b);
  kid.birth(a, b, parameters);
  BOOST_CHECK(kid != a);
  BOOST_CHECK(kid != b);
}
