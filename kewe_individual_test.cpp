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

BOOST_AUTO_TEST_CASE(kewe_individual_stream_out)
{
  const indiv i = create_test_individual();
  std::stringstream s;
  s << i;
  BOOST_CHECK(s.str() != "STUB");
}

BOOST_AUTO_TEST_CASE(kewe_create_offspring_is_reproducible)
{
  kewe_parameters parameters = create_test_parameters();
  const indiv father = create_test_individual();
  const indiv mother = create_test_individual();
  std::mt19937 rng_engine_1(42);
  const indiv kid_1 = create_offspring(father, mother, parameters, rng_engine_1);
  std::mt19937 rng_engine_2(42);
  const indiv kid_2 = create_offspring(father, mother, parameters, rng_engine_2);
  BOOST_CHECK(kid_1 == kid_2);
  std::mt19937 rng_engine_3(314);
  const indiv kid_3 = create_offspring(father, mother, parameters, rng_engine_3);
  BOOST_CHECK(kid_3 != kid_1);
  BOOST_CHECK(kid_3 != kid_2);
}

BOOST_AUTO_TEST_CASE(kewe_individual_throws_too_few_alleles)
{
  kewe_parameters parameters;
  parameters.sim_parameters.diploid = 1;
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



