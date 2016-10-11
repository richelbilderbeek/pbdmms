#include "ribi_population_factory.h"

#include <sstream>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace ribi;

BOOST_AUTO_TEST_CASE(test_ribi_population_factory)
{
  {
    const population p = population_factory().create_population_no_individuals();
    BOOST_CHECK(p.empty());
  }
  {
    const population p = population_factory().create_population_one_individual();
    BOOST_CHECK(p.size() == 1);
  }
  {
    const population p = population_factory().create_population_two_individuals_one_species();
    BOOST_CHECK(p.size() == 2);
  }
  {
    const population p = population_factory().create_population_two_individuals_two_species();
    BOOST_CHECK(p.size() == 2);
  }
  {
    const population p = population_factory().create_population_three_individuals_two_species();
    BOOST_CHECK(p.size() == 3);
  }
  {
    const population p = population_factory().create_population_three_individuals_one_species_one_possible_species();
    BOOST_CHECK(p.size() == 3);
  }
}



#pragma GCC diagnostic pop

