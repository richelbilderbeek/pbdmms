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

using namespace kewe;

/*BOOST_AUTO_TEST_CASE(kewe_individual_stream_out)
{
  const indiv i = create_test_individual();
  std::stringstream s;
  s << i;
  BOOST_CHECK(s.str() != "STUB");
}

BOOST_AUTO_TEST_CASE(kewe_create_offspring_is_reproducible)
{
  kewe::parameters parameters = create_test_parameters();
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
*/

BOOST_AUTO_TEST_CASE(kewe_individual_throws_too_few_alleles)
{
  kewe::parameters parameters;
  parameters.m_sim_parameters.diploid = 1;
  parameters.m_sim_parameters.Nx = 1;

  indiv a(parameters);
  indiv b(parameters);
  indiv test_kid1(parameters);

  std::mt19937 gen(parameters.m_sim_parameters.seed);

  BOOST_CHECK_THROW(test_kid1.birth(a, b, parameters, gen), std::invalid_argument);

  parameters.m_sim_parameters.Nx = 2;
  parameters.m_sim_parameters.Np = 1;

  indiv c(parameters);
  indiv d(parameters);
  indiv test_kid2(parameters);
  BOOST_CHECK_THROW(test_kid2.birth(c, d, parameters, gen), std::invalid_argument);

  parameters.m_sim_parameters.Np = 2;
  parameters.m_sim_parameters.Nq = 1;

  indiv e(parameters);
  indiv f(parameters);
  indiv test_kid3(parameters);
  BOOST_CHECK_THROW(test_kid3.birth(a, b, parameters, gen), std::invalid_argument);
}

#pragma GCC diagnostic pop

BOOST_AUTO_TEST_CASE(test_kewe_kid_birth_looks_like_parents)
{
  kewe::parameters parameters;
  indiv a(parameters);
  indiv b(parameters);

  std::mt19937 gen(parameters.m_sim_parameters.seed);

  BOOST_CHECK(a == b);

  indiv kid(parameters);
  kid.birth(a,b,parameters, gen);

  BOOST_CHECK(kid.get_fem_pref() >= - parameters.m_sim_parameters.sv * 4);
  BOOST_CHECK(kid.get_fem_pref() <= parameters.m_sim_parameters.sv * 4);

  BOOST_CHECK(kid.get_male_trait() >= - parameters.m_sim_parameters.sv * 4);
  BOOST_CHECK(kid.get_male_trait() <= parameters.m_sim_parameters.sv * 4);

}

BOOST_AUTO_TEST_CASE(test_os_operator_individual)
{
  kewe::parameters parameters;
  indiv a(parameters);
  std::cout << a << '\n';
}

/*BOOST_AUTO_TEST_CASE(test_random_normal_distribution_correct_range)
{
  std::mt19937 gen(42);
  std::normal_distribution<double> n_dis(0.0,0.02);
  for (int i = 0; i < 1000; ++i)
    {
      double random = n_dis(gen);
      std::cout << random << ' ';
      BOOST_CHECK(random >= -0.08 && random <= 0.08);
    }
  std::cout << '\n';
}
*/
