#include "jobo_individual.h"
#include <cassert>
#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <boost/test/unit_test.hpp>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

using namespace jobo;

BOOST_AUTO_TEST_CASE(test_jobo_individual_has_a_genotype)
{
  //An individual has a genotype
  const std::string genotype("ab");
  const individual i(genotype);
  BOOST_CHECK_EQUAL(i.get_genotype(), genotype);
}

BOOST_AUTO_TEST_CASE(test_jobo_calc_fitness_abuse)
{
  //Fitness calculation for genotypes of odd lengths should throw an exception
  BOOST_CHECK_THROW(calc_fitness("abc"), std::invalid_argument);

  //Genotypes should be letters only
  BOOST_CHECK_THROW(calc_fitness("  "), std::invalid_argument);
  BOOST_CHECK_THROW(calc_fitness("++"), std::invalid_argument);
  BOOST_CHECK_THROW(calc_fitness("--"), std::invalid_argument);
  BOOST_CHECK_THROW(calc_fitness("()"), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(test_jobo_cannot_create_negative_initial_genotype)
{
  BOOST_CHECK_THROW(
    create_initial_genotype(-2),
    std::invalid_argument
  );
}

BOOST_AUTO_TEST_CASE(test_jobo_genotype_has_number_of_loci)
{
  //A genotype has a number of loci
  const std::string genotype("ab");
  const individual i(genotype);
  BOOST_CHECK_EQUAL(i.get_n_loci(), genotype.size());
}

BOOST_AUTO_TEST_CASE(test_jobo_copy_individual_is_identical)
{
   //A copy of an individual is identical
   const std::string genotype("ab");
   const individual i(genotype);
   const individual j(i);
   BOOST_CHECK_EQUAL(i,j);
}

BOOST_AUTO_TEST_CASE(test_jobo_fitness_calculation_of_genotype)
{
   //Fitness calculation of genotype with 2 characters
   BOOST_CHECK_EQUAL(calc_fitness("ab"),1.0);
   BOOST_CHECK_EQUAL(calc_fitness("Ab"),1.0);
   BOOST_CHECK_EQUAL(calc_fitness("aB"),0.0);
   BOOST_CHECK_EQUAL(calc_fitness("AB"),1.0);

   //Fitness calculation of genotype with 2 characters
   BOOST_CHECK_EQUAL(calc_fitness("ab"),1.0);
   BOOST_CHECK_EQUAL(calc_fitness("Ab"),1.0);
   BOOST_CHECK_EQUAL(calc_fitness("aB"),0.0);
   BOOST_CHECK_EQUAL(calc_fitness("AB"),1.0);
}

BOOST_AUTO_TEST_CASE(test_jobo_fitness_calculation_of_4_character_genotype)
{
   //Fitness calculation of genotype with 4 characters
   BOOST_CHECK_EQUAL(calc_fitness("abcd"),1.0);
   BOOST_CHECK_EQUAL(calc_fitness("AbCd"),1.0);
   BOOST_CHECK_EQUAL(calc_fitness("aBCD"),0.0);
   BOOST_CHECK_EQUAL(calc_fitness("AbcD"),0.0);
   BOOST_CHECK_EQUAL(calc_fitness("abCD"),1.0);
   BOOST_CHECK_EQUAL(calc_fitness("ABcd"),1.0);
   BOOST_CHECK_EQUAL(calc_fitness("aBcD"),0.0);
}

BOOST_AUTO_TEST_CASE(test_jobo_fitness_calculation_of_6_character_genotype)
{
   //Fitness calculation of genotype with 6 characters
   BOOST_CHECK_EQUAL(calc_fitness("abcdef"),1.0);
   BOOST_CHECK_EQUAL(calc_fitness("AbCdEf"),1.0);
   BOOST_CHECK_EQUAL(calc_fitness("ABCDeF"),0.0);
   BOOST_CHECK_EQUAL(calc_fitness("ABcdef"),1.0);
   BOOST_CHECK_EQUAL(calc_fitness("abCDef"),1.0);
   BOOST_CHECK_EQUAL(calc_fitness("abcdEF"),1.0);
}

BOOST_AUTO_TEST_CASE(test_jobo_recombination)
{
   // Test recombination function with two complete different parents
   std::mt19937 rng_engine(42);
   const genotype p("abcdefghijklmnopqrstuvwxyz");
   const genotype q("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
   const genotype r = recombine(p, q, rng_engine);
   BOOST_CHECK (r!=p);
   BOOST_CHECK (r!=q);
}

BOOST_AUTO_TEST_CASE(test_jobo_create_offspring_with_recombined_genotype)
{
    // Create_offspring with recombined genotype
    std::mt19937 rng_engine(42);
    const genotype p("abcdefghijklmnopqrstuvwxyz");
    const genotype q("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    const individual mother(p);
    const individual father(q);
    const individual child = create_offspring(mother,father,rng_engine);
    const genotype r = child.get_genotype();
    BOOST_CHECK (child!=mother);
    BOOST_CHECK (child!=father);
    BOOST_CHECK (r!=p);
    BOOST_CHECK (r!=q);
}

BOOST_AUTO_TEST_CASE(test_jobo_create_offspring_with_uneven_genotype)
{
    // Create_offspring with recombined genotype
    std::mt19937 rng_engine(42);
    const genotype p("abcdefghijklmnopqrstuvwxy");
    const genotype q("ABCDEFGHIJKLMNOPQRSTUVWXY");
    const individual mother(p);
    const individual father(q);
    BOOST_CHECK_THROW(create_offspring(mother,father,rng_engine), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(test_jobo_mutation_check_all_loci)
{
    // Test Mutation_check_all_loci function for genotype of 26 loci
    std::mt19937 rng_engine(42);
    const double mutation_rate{0.5};
    const genotype r("aBcDeFgHiJkLmNoPqRsTuVwXyZ");
    const genotype v = mutation_check_all_loci(r,mutation_rate,rng_engine);
    BOOST_CHECK (r!=v);
}

BOOST_AUTO_TEST_CASE(test_jobo_create_mutation)
{
    // Create individual with mutation with create_mutation function
    std::mt19937 rng_engine(42);
    const double mutation_rate{0.5};
    const genotype r("AbCdEfGhIjKlMnOpQrStUvWxYz");
    const individual before_mutation(r);
    const individual after_mutation = create_mutation(before_mutation,mutation_rate,rng_engine);
    BOOST_CHECK (before_mutation!=after_mutation);
}

BOOST_AUTO_TEST_CASE(test_jobo_recombine)
{
    // Create individual with mutation with create_mutation function
    std::mt19937 rng_engine(42);
    const genotype i("AbCdEfGhIjKlMnOpQrStUvWxYz");
    const genotype j("AbCdEfGhIjKlMnOpQrStUvWxY");
    BOOST_CHECK_THROW(
      recombine(i,j,rng_engine),
      std::invalid_argument
    );
}

BOOST_AUTO_TEST_CASE(test_is_viable_species)
{
  const genotype r("AbCdEfGhIjKlMnOpQrStUvWxYz");
  BOOST_CHECK_EQUAL ((is_viable_species(r)),1);
  const genotype q("AbCdEfGhIjKlmNOpQrStUvWxYz");
  BOOST_CHECK_EQUAL ((is_viable_species(q)),0);
  const genotype s("aBCdEfGhIjKlmNOpQrStUvWxYz");
  BOOST_CHECK_EQUAL ((is_viable_species(q)),0);
}

BOOST_AUTO_TEST_CASE(test_jobo_create_offspring)
{
    // Create individual with mutation with create_mutation function
    std::mt19937 rng_engine(42);
    const genotype i("AbCdEfGhIjKlMnOpQrStUvWxYz");
    const genotype j("AbCdEfGhIjKlMnOpQrStUvWxY");
    const individual k(i);
    const individual l(j);
    BOOST_CHECK_THROW(
      create_offspring(k,l,rng_engine),
      std::invalid_argument
    );
}
#pragma GCC diagnostic pop

