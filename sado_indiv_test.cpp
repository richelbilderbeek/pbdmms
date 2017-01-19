#include "sado_individual.h"

#include <fstream>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace sado;

BOOST_AUTO_TEST_CASE(sado_indiv_have_different_ids)
{
  const indiv a;
  const indiv b;
  BOOST_CHECK(a.get_id() != b.get_id());
}

#pragma GCC diagnostic pop



