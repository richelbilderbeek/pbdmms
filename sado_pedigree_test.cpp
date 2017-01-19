#include "sado_pedigree.h"

#include <fstream>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace sado;

BOOST_AUTO_TEST_CASE(sado_pedigree_default_construction)
{
  pedigree p;
  BOOST_CHECK(p.empty());
}

#pragma GCC diagnostic pop

