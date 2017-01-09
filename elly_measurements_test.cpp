#include "elly_measurements.h"

#include "elly_parameters.h"
#include "elly_simulation.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace elly;

#ifdef ELLY_TODO_LATER
BOOST_AUTO_TEST_CASE(elly_collect_measurements)
{
  const auto p = create_parameters_set2();
  simulation s(p);
  s.run();
  //collect_ts
  {
    BOOST_CHECK(!collect_ts(s.get_measurements()).empty());
  }
  //collect for events
  {
    const auto es = collect_all_events();
    for (const auto e: es)
    {
      BOOST_CHECK(!collect(s.get_measurements(), e).empty());
    }
  }
  //collect for locations
  {
    const auto locations = collect_all_locations();
    for (const auto where: locations)
    {
      BOOST_CHECK(!collect(s.get_measurements(), where).empty());
    }
  }
  //collect_extinct
  {
    BOOST_CHECK(!collect_extinct(s.get_measurements()).empty());
  }
  //to_raw
  {
    const auto es = collect_all_events();
    for (const auto e: es)
    {
      BOOST_CHECK(!to_raw(collect(s.get_measurements(), e)).empty());
    }
  }
}
#endif //ifdef ELLY_TODO_LATER

#pragma GCC diagnostic pop

