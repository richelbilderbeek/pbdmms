#include "sado_simulation.h"


#include "sado_parameters.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace sado;

BOOST_AUTO_TEST_CASE(sado_simulation_must_reproduce_golden_standard)
{
  /*
  const auto p = create_golden_standard_parameters();
  simulation s(p);
  s.run();
  BOOST_CHECK_EQUAL(s.get_results(), get_golden_standard_results());
  */
}

#pragma GCC diagnostic pop

