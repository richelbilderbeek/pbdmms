#include "distancer_species_id.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_species_id)
{
  const species_id a;
  const species_id a_too{a};
  const species_id b;
  const species_id b_too{b};
  BOOST_CHECK_EQUAL(a, a_too);
  BOOST_CHECK_EQUAL(b, b_too);
  BOOST_CHECK_NE(a, b_too);
  BOOST_CHECK_NE(b, a_too);
}

