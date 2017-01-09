#include "elly_location.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace elly;

BOOST_AUTO_TEST_CASE(elly_collect_all_locations)
{
  const auto v = collect_all_locations();
  BOOST_CHECK(!v.empty());
}

BOOST_AUTO_TEST_CASE(elly_location_to_str)
{
  const auto v = collect_all_locations();
  for (const location any_location: v)
  {
    BOOST_CHECK(!to_str(any_location).empty());
  }
}

#pragma GCC diagnostic pop









