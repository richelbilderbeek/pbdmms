#include <fstream>
#include <iostream>
#include <random>
#include "kewe_parameters.h"
#include "kewe_individual.h"
#include "kewe_individuals.h"
#include <stdexcept>
#include <cassert>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace kewe;

BOOST_AUTO_TEST_CASE(kewe_create_test_individuals_1)
{
  ///Creates a population with these phenotypes:
  /// +---+-------------------+------------+---------+
  /// | # | female_preference | male_trait | ecotype |
  /// +---+-------------------+------------+---------+
  /// | a |       1.0         |    1.0     |   1.0   |
  /// | b |       1.0         |    2.0     |   1.0   |
  /// | c |       1.0         |    3.0     |   1.0   |
  /// +---+-------------------+------------+---------+
  const individuals pop = create_test_individuals_1();
  BOOST_REQUIRE_EQUAL(pop.size(), 3);
  const auto& a = pop[0];
  const auto& b = pop[1];
  const auto& c = pop[2];
  BOOST_CHECK(std::abs(a.get_fem_pref() - 1.0) < 0.001);
  BOOST_CHECK(std::abs(b.get_fem_pref() - 1.0) < 0.001);
  BOOST_CHECK(std::abs(c.get_fem_pref() - 1.0) < 0.001);

  BOOST_CHECK(std::abs(a.get_male_trait() - 1.0) < 0.001);
  BOOST_CHECK(std::abs(b.get_male_trait() - 2.0) < 0.001);
  BOOST_CHECK(std::abs(c.get_male_trait() - 3.0) < 0.001);

  BOOST_CHECK(std::abs(a.get_eco_trait() - 1.0) < 0.001);
  BOOST_CHECK(std::abs(a.get_eco_trait() - 1.0) < 0.001);
  BOOST_CHECK(std::abs(a.get_eco_trait() - 1.0) < 0.001);
}

BOOST_AUTO_TEST_CASE(kewe_default_constructed_individuals_are_identical)
{
  const individual a;
  const individual b;
  BOOST_CHECK_EQUAL(a, a);
  BOOST_CHECK_EQUAL(a, b);
  BOOST_CHECK_EQUAL(b, a);
  BOOST_CHECK_EQUAL(b, b);
}

BOOST_AUTO_TEST_CASE(kewe_fuzzy_constructed_individuals_are_different)
{
  const simulation_parameters p;
  std::mt19937 gen(p.seed);
  const individual a(p, gen);
  const individual b(p, gen);
  BOOST_CHECK_NE(a, b);

}

BOOST_AUTO_TEST_CASE(kewe_initial_individuals_are_different)
{
  {
    const individual a = individual(0.0, 0.0, 0.0, {0.0}, {0.0}, {0.0});
    const individual b = individual(0.0, 0.0, 0.0, {0.0}, {0.0}, {0.0});
    BOOST_CHECK_EQUAL(a, b);
  }
  //Differ on first
  {
    const individual a = individual(0.0, 0.0, 0.0, {0.0}, {0.0}, {0.0});
    const individual b = individual(1.0, 0.0, 0.0, {0.0}, {0.0}, {0.0});
    BOOST_CHECK_NE(a, b);
  }
  //Differ on second
  {
    const individual a = individual(0.0, 0.0, 0.0, {0.0}, {0.0}, {0.0});
    const individual b = individual(0.0, 1.0, 0.0, {0.0}, {0.0}, {0.0});
    BOOST_CHECK_NE(a, b);
  }
  //Differ on third
  {
    const individual a = individual(0.0, 0.0, 0.0, {0.0}, {0.0}, {0.0});
    const individual b = individual(0.0, 0.0, 1.0, {0.0}, {0.0}, {0.0});
    BOOST_CHECK_NE(a, b);
  }
  //Differ on fourth
  {
    const individual a = individual(0.0, 0.0, 0.0, {0.0}, {0.0}, {0.0});
    const individual b = individual(0.0, 0.0, 0.0, {1.0}, {0.0}, {0.0});
    BOOST_CHECK_NE(a, b);
  }
  //Differ on fifth
  {
    const individual a = individual(0.0, 0.0, 0.0, {0.0}, {0.0}, {0.0});
    const individual b = individual(0.0, 0.0, 0.0, {0.0}, {1.0}, {0.0});
    BOOST_CHECK_NE(a, b);
  }
  //Differ on second
  {
    const individual a = individual(0.0, 0.0, 0.0, {0.0}, {0.0}, {0.0});
    const individual b = individual(0.0, 0.0, 0.0, {0.0}, {0.0}, {1.0});
    BOOST_CHECK_NE(a, b);
  }
}

BOOST_AUTO_TEST_CASE(kewe_create_offspring_should_give_offspring_different_from_parents)
{
  const individual a;
  const individual b;
  assert(a == b);

  const simulation_parameters p;
  std::mt19937 gen(p.seed);
  const individual kid = create_offspring(a, b, p, gen);
  BOOST_CHECK_NE(kid, a);
  BOOST_CHECK_NE(kid, b);
}

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
  simulation_parameters p;
  std::mt19937 gen(p.seed);
  p.diploid = 1;
  p.Nx = 1;

  const individual a(p, gen);
  const individual b(p, gen);
  individual test_kid1(p, gen);


  BOOST_CHECK_THROW(test_kid1.birth(a, b, p, gen), std::invalid_argument);

  p.Nx = 2;
  p.Np = 1;

  const individual c(p, gen);
  const individual d(p, gen);
  individual test_kid2(p, gen);
  BOOST_CHECK_THROW(test_kid2.birth(c, d, p, gen), std::invalid_argument);

  p.Np = 2;
  p.Nq = 1;

  const individual e(p, gen);
  const individual f(p, gen);
  individual test_kid3(p, gen);
  BOOST_CHECK_THROW(test_kid3.birth(a, b, p, gen), std::invalid_argument);
}


BOOST_AUTO_TEST_CASE(kewe_kid_birth_looks_like_parents)
{
  const simulation_parameters p;
  std::mt19937 gen(p.seed);

  const individual a(p, gen);
  const individual b(p, gen);

  BOOST_CHECK(a == b);

  individual kid(p, gen);
  kid.birth(a,b,p, gen);

  BOOST_CHECK(kid.get_fem_pref() >= - p.sv * 4);
  BOOST_CHECK(kid.get_fem_pref() <= p.sv * 4);

  BOOST_CHECK(kid.get_male_trait() >= - p.sv * 4);
  BOOST_CHECK(kid.get_male_trait() <= p.sv * 4);

}

BOOST_AUTO_TEST_CASE(os_operator_individual)
{
  const simulation_parameters p;
  std::mt19937 gen(p.seed);
  const individual a(p, gen);
  std::stringstream s;
  s << a << '\n';
  BOOST_CHECK(!s.str().empty());
}

/*BOOST_AUTO_TEST_CASE(random_normal_distribution_correct_range)
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
