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


BOOST_AUTO_TEST_CASE(test_kewe_initial_individuals_are_different)
{
  kewe_parameters parameters;
  std::vector<indiv> pop = initialize(parameters);

  indiv a = pop[1];
  indiv b = pop[2];

  BOOST_CHECK(a != b);

}
