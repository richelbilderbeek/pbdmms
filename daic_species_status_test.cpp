#include "daic_species_status.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace daic;

BOOST_AUTO_TEST_CASE(daic_to_str_and_to_species_status_must_be_symmetrical)
{
  BOOST_CHECK(1 == 2);
}


#pragma GCC diagnostic pop




