#include "sado_newick.h"

#include <fstream>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace sado;

BOOST_AUTO_TEST_CASE(sado_is_newick)
{
  BOOST_CHECK(!is_newick("nonsense"));
  BOOST_CHECK( is_newick("(:0.1,:0.2,(:0.3,:0.4):0.5);"));
}

BOOST_AUTO_TEST_CASE(sado_to_newick_3)
{
  /*
   [2]
    |
    |
   [1]
    |
    |
   [0]
  */
  const auto g = create_test_graph_3();
  BOOST_CHECK_EQUAL(to_newick(g), "(:2);");
}


#pragma GCC diagnostic pop



