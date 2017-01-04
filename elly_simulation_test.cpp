#include "elly_simulation.h"

#include "elly_parameters.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace elly;

BOOST_AUTO_TEST_CASE(elly_run_simulation)
{
  const parameters p = create_parameters_set1();
  simulation s(p);
  BOOST_CHECK_NO_THROW(s.run());
}

BOOST_AUTO_TEST_CASE(elly_run_simulation_with_profiling_parameters)
{
  const parameters p = create_profiling_parameters();
  simulation s(p);
  BOOST_CHECK_NO_THROW(s.run());
}


#pragma GCC diagnostic pop
