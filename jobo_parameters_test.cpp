#include "jobo_parameters_test.h"
#include "jobo_parameters.h"
#include <cassert>
#include <exception>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <boost/test/unit_test.hpp>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

using namespace jobo;

BOOST_AUTO_TEST_CASE(test_jobo_create_parameter_settings)
{
    //Create tests for parameter settings
    const int n_loci{42};
    const int population_size{1000};
    const int seed{38};
    const double mutation_rate{0.5};
    const int duration{10};
    parameters p(n_loci, population_size,seed,mutation_rate,duration);
    BOOST_CHECK_EQUAL(p.get_n_loci(),n_loci);
    BOOST_CHECK_EQUAL(p.get_population_size(),population_size);
    BOOST_CHECK_EQUAL(p.get_seed(),seed);
    BOOST_CHECK_EQUAL(p.get_duration(),duration);
}

BOOST_AUTO_TEST_CASE(test_jobo_cannot_have_negative_number_of_loci)
{
    //Cannot have a negative number of loci
    const int n_loci{-1234};
    const int population_size{1000};
    const int seed{38};
    const double mutation_rate{0.5};
    const int duration{10};
    BOOST_CHECK_THROW(
      parameters(n_loci, population_size,seed,mutation_rate,duration),
      std::invalid_argument
   );
}

BOOST_AUTO_TEST_CASE(test_jobo_cannot_have_negative_population_size)
{
    //Cannot have a negative population_size
    const int n_loci{42};
    const int population_size{-1234};
    const int seed{38};
    const double mutation_rate{0.5};
    const int duration{10};
    BOOST_CHECK_THROW(
      parameters(n_loci, population_size,seed,mutation_rate,duration),
      std::invalid_argument
    );
}

BOOST_AUTO_TEST_CASE(test_jobo_cannot_have_negative_duration)
{
    //Cannot have a negative population_size
    const int n_loci{42};
    const int population_size{1000};
    const int seed{38};
    const double mutation_rate{0.5};
    const int duration{-10};
    BOOST_CHECK_THROW(
      parameters(n_loci, population_size,seed,mutation_rate,duration),
      std::invalid_argument
    );
}

BOOST_AUTO_TEST_CASE(test_jobo_mutation_rate_must_be_zero_at_least)
{
    const int n_loci{10};
    const int population_size{10};
    const int seed{42};
    const int duration{10};
    BOOST_CHECK_NO_THROW(
      parameters(
        n_loci,
        population_size,
        seed,
        0.0,
        duration
      )
    );
    BOOST_CHECK_THROW(
      parameters(
        n_loci,
        population_size,
        seed,
        -0.1,
        duration
      ),
      std::invalid_argument
    );
}

BOOST_AUTO_TEST_CASE(test_jobo_mutation_rate_must_be_one_at_most)
{
    const int n_loci{10};
    const int population_size{10};
    const int seed{42};
    const int duration{10};
    BOOST_CHECK_NO_THROW(
      parameters(
        n_loci,
        population_size,
        seed,
        1.0,
        duration
      )
    );
    BOOST_CHECK_THROW(
      parameters(
        n_loci,
        population_size,
        seed,
        1.1,
        duration
      ),
      std::invalid_argument
    );
}


BOOST_AUTO_TEST_CASE(test_jobo_parameters_copy_and_equality)
{
    //Parameters copy and equality
    const parameters a(42,1,38,0.5,10);
    const parameters b(a); //Copy
    const parameters c( 0,1,38,0.5,10);
    const parameters d(42,0,38,0.5,10);
    BOOST_CHECK(a==a);
    BOOST_CHECK(a==b);
    BOOST_CHECK(a!=c);
    BOOST_CHECK(a!=d);

    BOOST_CHECK(b==a);
    BOOST_CHECK(b==b);
    BOOST_CHECK(b!=c);
    BOOST_CHECK(b!=d);

    BOOST_CHECK(c!=a);
    BOOST_CHECK(c!=b);
    BOOST_CHECK(c==c);
    BOOST_CHECK(c!=d);

    BOOST_CHECK(d!=a);
    BOOST_CHECK(d!=b);
    BOOST_CHECK(d!=c);
    BOOST_CHECK(d==d);
}

#pragma GCC diagnostic pop





