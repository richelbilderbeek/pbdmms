#include "sado_population.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace sado;

BOOST_AUTO_TEST_CASE(sado_population_default_construction)
{
  population p;
  BOOST_CHECK(p.empty());
}

#pragma GCC diagnostic pop


