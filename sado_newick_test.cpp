#include "sado_newick.h"

#include <fstream>

#include <boost/test/unit_test.hpp>

#include "sado_parameters.h"

using namespace sado;

BOOST_AUTO_TEST_CASE(sado_is_newick)
{
  BOOST_CHECK(!is_newick("nonsense"));
  BOOST_CHECK(!is_newick("((:1,:1));"));
  BOOST_CHECK(!is_newick("((:2,(:1,:1):1));"));

  BOOST_CHECK( is_newick("(:1,:1):1;")); //According to ape
  BOOST_CHECK( is_newick("(:0.1,:0.2,(:0.3,:0.4):0.5);"));
}

BOOST_AUTO_TEST_CASE(sado_to_newick_7)
{
  /*
   [0]
  */
  const auto g = create_test_graph_7();
  BOOST_CHECK_EQUAL(to_newick(g), "(:0);");
  BOOST_CHECK(is_newick(to_newick(g)));
}

BOOST_AUTO_TEST_CASE(sado_to_newick_6)
{
  /*
   [1]
    |
    |
   [0]
  */
  const auto g = create_test_graph_6();
  BOOST_CHECK_EQUAL(to_newick(g), "(:1);");
  BOOST_CHECK(is_newick(to_newick(g)));
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
  BOOST_CHECK(is_newick(to_newick(g)));
}

BOOST_AUTO_TEST_CASE(sado_to_newick_18)
{
  /*

    1 +  {b} {c}
      |   |  /
      |   | /
    0 +  {a}

    t (generation)

  */
  const auto g = create_test_graph_18();

  assert(count_n_extant(g) == 2);
  assert(count_n_generations(g) == 2);
  assert(collect_root_vds(g).size() == 1);
  const auto root_vd = collect_root_vds(g).back();
  BOOST_CHECK_EQUAL(to_newick(root_vd, g), "(:1,:1)");
  BOOST_CHECK_EQUAL(to_newick(g), "(:1,:1);");
  BOOST_CHECK(is_newick(to_newick(g)));
}


BOOST_AUTO_TEST_CASE(sado_to_newick_16)
{
  /*

     [3] [4]
      |   |
      |   |
     [1] [2]
      | /
      |/
     [0]

  */

  const auto g = create_test_graph_16();
  assert(count_n_extant(g) == 2);
  assert(count_n_generations(g) == 3);
  assert(collect_root_vds(g).size() == 1);
  const auto root_vd = collect_root_vds(g).back();
  BOOST_CHECK_EQUAL(to_newick(root_vd, g), "(:2,:2)");

  BOOST_CHECK_EQUAL(to_newick(g), "(:2,:2);");
  BOOST_CHECK(is_newick(to_newick(g)));
}

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
  assert(count_n_generations(g) == 3);
  assert(collect_root_vds(g).size() == 1);
  const auto root_vd = collect_root_vds(g).back();
  BOOST_CHECK_EQUAL(to_newick(root_vd, g), "(:1,:1):1");

  BOOST_CHECK_EQUAL(to_newick(g), "(:1,:1):1;");
  BOOST_CHECK(is_newick(to_newick(g)));
}

BOOST_AUTO_TEST_CASE(sado_to_newick_19)
{
  /*

   2 +    [3]   [4]   [5]
     |      \     \   /
     |       \     \ /
   1 +       [1]   [2]
     |         \   /
     |          \ /
   0 +          [0]

     t (generation)
  */
  const auto g = create_test_graph_19();
  assert(count_n_extant(g) == 3);
  assert(count_n_generations(g) == 3);
  assert(collect_root_vds(g).size() == 1);
  const auto root_vd = collect_root_vds(g).back();
  //Newicks can also be the other way around
  BOOST_CHECK_EQUAL(to_newick(root_vd, g), "(:2,(:1,:1):1)");
  BOOST_CHECK_EQUAL(to_newick(g), "(:2,(:1,:1):1);");
  BOOST_CHECK(is_newick(to_newick(g)));
}

BOOST_AUTO_TEST_CASE(sado_all_reconstructed_phylogenies_can_be_valid_newick)
{
  for (const auto g: create_test_graphs())
  {
    const auto r = create_reconstructed(g);
    const auto s = to_newick(r);
    BOOST_CHECK(is_newick(s));
  }
}

BOOST_AUTO_TEST_CASE(sado_to_newick_impls_are_identical)
{
  for (const auto g: create_test_graphs())
  {
    const auto r = create_reconstructed(g);
    const auto s = to_newick_impl1(r);
    const auto t = to_newick_impl2(r);
    BOOST_CHECK_EQUAL(s, t);
  }
}
