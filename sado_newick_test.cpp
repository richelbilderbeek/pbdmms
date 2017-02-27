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

#ifdef FIX_ISSUE_237_EXTREMELY_WELL
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
#endif //REALLY_TEST_THIS

#ifdef FIX_ISSUE_237
BOOST_AUTO_TEST_CASE(sado_to_newick_2)
{
  /*
   [2]  [3]
    |  /
    | /
   [1]
    |
    |
   [0]
  */
  const auto g = create_test_graph_2();
  assert(count_n_extant(g) == 2);
  BOOST_CHECK_EQUAL(to_newick(g), "((:1,:1):1);");
}
#endif // FIX_ISSUE_237


#pragma GCC diagnostic pop



