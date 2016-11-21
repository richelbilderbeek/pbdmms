#include <fstream>
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <random>
#include "kewe_parameters.h"
#include "kewe_individual.h"
#include <stdexcept>
#include <cassert>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

BOOST_AUTO_TEST_CASE(kewe_individual_throws_too_few_alleles)
{
  kewe_parameters parameters;
  parameters.sim_parameters.Nx = 1;

  indiv a(parameters);
  indiv b(parameters);
  indiv test_kid1(parameters);

  std::cout << static_cast<int>(a.getX().size()) << '\n';
  std::cout << static_cast<int>(b.getX().size()) << '\n';
  std::cout << static_cast<int>(test_kid1.getX().size()) << '\n';

  BOOST_CHECK_THROW(test_kid1.birth(a, b, parameters), std::invalid_argument);

  parameters.sim_parameters.Nx = 2;
  parameters.sim_parameters.Np = 1;

  indiv c(parameters);
  indiv d(parameters);
  indiv test_kid2(parameters);
  BOOST_CHECK_THROW(test_kid2.birth(c, d, parameters), std::invalid_argument);

  parameters.sim_parameters.Np = 2;
  parameters.sim_parameters.Nq = 1;

  indiv e(parameters);
  indiv f(parameters);
  indiv test_kid3(parameters);
  BOOST_CHECK_THROW(test_kid3.birth(a, b, parameters), std::invalid_argument);
}

#pragma GCC diagnostic pop



