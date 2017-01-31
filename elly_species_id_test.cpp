#include "elly_species_id.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace elly;

BOOST_AUTO_TEST_CASE(elly_create_negative_of_species_id)
{
  const species_id a = create_new_species_id();
  const species_id b = create_negative_of(a);
  BOOST_CHECK_EQUAL(a.get_id(), -b.get_id());
}

#pragma GCC diagnostic pop

