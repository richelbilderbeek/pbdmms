#include "phyg_create_reconstructed.h"

#include <fstream>

#include "jobo_ancestry_graph.h"
#include "kewe_ancestry_graph.h"
#include "sado_ancestry_graph.h"
#include "is_isomorphic.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace phyg;

BOOST_AUTO_TEST_CASE(phyg_create_reconstructed_jobo)
{
  const auto g = jobo::create_test_graph_2();
  const auto r = phyg::create_reconstructed(g);
  BOOST_CHECK(is_isomorphic(g, r));
}

BOOST_AUTO_TEST_CASE(phyg_create_reconstructed_kewe)
{
  const auto g = kewe::create_test_graph_2();
  const auto r = phyg::create_reconstructed(g);
  BOOST_CHECK(is_isomorphic(g, r));
}

BOOST_AUTO_TEST_CASE(phyg_create_reconstructed_sado)
{
  const auto g = sado::create_test_graph_2();
  const auto r = phyg::create_reconstructed(g);
  BOOST_CHECK(is_isomorphic(g, r));
}

#pragma GCC diagnostic pop
