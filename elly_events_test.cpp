#include "elly_events.h"


// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace elly;

BOOST_AUTO_TEST_CASE(elly_collect_all_events)
{
  const auto v = collect_all_events();
  BOOST_CHECK(!v.empty());
}

BOOST_AUTO_TEST_CASE(elly_event_to_str)
{
  const auto v = collect_all_events();
  for (const event e: v)
  {
    BOOST_CHECK(!to_str(e).empty());
  }
}

#pragma GCC diagnostic pop








