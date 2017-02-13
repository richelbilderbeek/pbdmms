#include "sado_species.h"
#include "sado_individual.h"

#include <fstream>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace sado;

BOOST_AUTO_TEST_CASE(sado_species_test_trivial_functions)
{
  sado_species test_species;
  BOOST_CHECK(test_species.empty());

  const indiv i;
  test_species.add_indiv(i);
  BOOST_CHECK(static_cast<int>(test_species.size()) == 1);
  const indiv j;
  test_species.add_indiv(j);

  BOOST_CHECK(static_cast<int>(test_species.size()) == 2);
  BOOST_CHECK(i != j);
  BOOST_CHECK(test_species[0] != j);
  BOOST_CHECK(test_species[0] == i);
  BOOST_CHECK(test_species[1] == j);
  BOOST_CHECK(test_species[1] != i);
}

BOOST_AUTO_TEST_CASE(sado_species_get_right_gen_number)
{
  sado_species test_species(42);
  BOOST_CHECK_EQUAL(test_species.get_generation(), 42);
}



#pragma GCC diagnostic pop
