#include "jaan_individual.h"

#include "jaan_parameters.h"

#include <random>
#include <boost/test/unit_test.hpp>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

/*
BOOST_AUTO_TEST_CASE(jaan_individual)
{
  const jaan_parameters p;
  std::mt19937 generator(42);
  const Individual i(p, generator);
  const Individual j(p, generator);
  BOOST_CHECK(i != j);

}*/

BOOST_AUTO_TEST_CASE(jaan_individual)
{
  Parameters p;
  std::mt19937 generator(42);
  const Individual i(p, generator);
  const Individual j(i);
  BOOST_CHECK(i == j);
}

#pragma GCC diagnostic pop



