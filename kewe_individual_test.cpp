#include <fstream>
#include <iostream>
#include <random>
#include "kewe_parameters.h"
#include "kewe_individual.h"
#include <stdexcept>
#include <cassert>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace kewe;

/*BOOST_AUTO_TEST_CASE(kewe_individual_stream_out)
{
  const individual i = create_test_individual();
  std::stringstream s;
  s << i;
  BOOST_CHECK(s.str() != "STUB");
}

BOOST_AUTO_TEST_CASE(kewe_create_offspring_is_reproducible)
{
  kewe::parameters parameters = create_test_parameters();
  const individual father = create_test_individual();
  const individual mother = create_test_individual();
  std::mt19937 rng_engine_1(42);
  const individual kid_1 = create_offspring(father, mother, parameters, rng_engine_1);
  std::mt19937 rng_engine_2(42);
  const individual kid_2 = create_offspring(father, mother, parameters, rng_engine_2);
  BOOST_CHECK(kid_1 == kid_2);
  std::mt19937 rng_engine_3(314);
  const individual kid_3 = create_offspring(father, mother, parameters, rng_engine_3);
  BOOST_CHECK(kid_3 != kid_1);
  BOOST_CHECK(kid_3 != kid_2);
}
*/

BOOST_AUTO_TEST_CASE(kewe_individual_throws_too_few_alleles)
{
  parameters p;
  p.m_sim_parameters.diploid = 1;
  p.m_sim_parameters.Nx = 1;

  individual a(p);
  individual b(p);
  individual test_kid1(p);

  std::mt19937 gen(p.m_sim_parameters.seed);

  BOOST_CHECK_THROW(test_kid1.birth(a, b, p, gen), std::invalid_argument);

  p.m_sim_parameters.Nx = 2;
  p.m_sim_parameters.Np = 1;

  individual c(p);
  individual d(p);
  individual test_kid2(p);
  BOOST_CHECK_THROW(test_kid2.birth(c, d, p, gen), std::invalid_argument);

  p.m_sim_parameters.Np = 2;
  p.m_sim_parameters.Nq = 1;

  individual e(p);
  individual f(p);
  individual test_kid3(p);
  BOOST_CHECK_THROW(test_kid3.birth(a, b, p, gen), std::invalid_argument);
}


BOOST_AUTO_TEST_CASE(test_kewe_kid_birth_looks_like_parents)
{
  parameters p;
  individual a(p);
  individual b(p);

  std::mt19937 gen(p.m_sim_parameters.seed);

  BOOST_CHECK(a == b);

  individual kid(p);
  kid.birth(a,b,p, gen);

  BOOST_CHECK(kid.get_fem_pref() >= - p.m_sim_parameters.sv * 4);
  BOOST_CHECK(kid.get_fem_pref() <= p.m_sim_parameters.sv * 4);

  BOOST_CHECK(kid.get_male_trait() >= - p.m_sim_parameters.sv * 4);
  BOOST_CHECK(kid.get_male_trait() <= p.m_sim_parameters.sv * 4);

}

BOOST_AUTO_TEST_CASE(test_os_operator_individual)
{
  parameters p;
  individual a(p);
  std::stringstream s;
  s << a << '\n';
  BOOST_CHECK(!s.str().empty());
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

#pragma GCC diagnostic pop
