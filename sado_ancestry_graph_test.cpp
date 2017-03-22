#include "sado_ancestry_graph.h"
#include "sado_species.h"
#include "sado_individual.h"
#include "sado_id.h"
#include "save_graph_to_dot.h"
#include "convert_dot_to_svg.h"
#include "convert_svg_to_png.h"
#include "sado_parameters.h"
#include "is_isomorphic.h"


#include <fstream>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace sado;

BOOST_AUTO_TEST_CASE(sado_create_test_graph_1)
{
  const auto g = create_test_graph_1();
  BOOST_CHECK_EQUAL(boost::num_vertices(g), 8);
  BOOST_CHECK_EQUAL(boost::num_edges(g), 11);
}

BOOST_AUTO_TEST_CASE(sado_create_test_graph_2)
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
  BOOST_CHECK_EQUAL(boost::num_vertices(g), 4);
  BOOST_CHECK_EQUAL(boost::num_edges(g), 3);

}

BOOST_AUTO_TEST_CASE(sado_count_n_extant_2)
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
  BOOST_CHECK_EQUAL(count_n_extant(g), 2);
}

BOOST_AUTO_TEST_CASE(sado_count_n_extant_3)
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
  BOOST_CHECK_EQUAL(count_n_extant(g), 1);
}

BOOST_AUTO_TEST_CASE(sado_count_n_extant_4)
{
/*
 [3]
  | \
  |  \
 [1]  [2]
  |  /
  | /
 [0]
*/
  const auto g = create_test_graph_4();
  BOOST_CHECK_EQUAL(count_n_extant(g), 1);
}

BOOST_AUTO_TEST_CASE(sado_count_n_extant_5)
{
/*
 [3]
  |
  |
 [1]  [2]
  |  /
  | /
 [0]
*/
  const auto g = create_test_graph_5();
  BOOST_CHECK_EQUAL(count_n_extant(g), 1);
}

BOOST_AUTO_TEST_CASE(sado_count_n_extant_6)
{
/*
 [1]
  |
  |
 [0]
*/
  const auto g = create_test_graph_6();
  BOOST_CHECK_EQUAL(count_n_extant(g), 1);
}

BOOST_AUTO_TEST_CASE(sado_count_n_extant_7)
{
/*

 [0]

*/
  const auto g = create_test_graph_7();
  BOOST_CHECK_EQUAL(count_n_extant(g), 1);
}


BOOST_AUTO_TEST_CASE(sado_is_tip_7)
{
  /*

   [0]

  */
  const auto g = create_test_graph_7();
  const sp_vert_desc vd_a = *boost::vertices(g).first;
  BOOST_CHECK(is_tip(vd_a, g));
}

BOOST_AUTO_TEST_CASE(sado_is_tip_6)
{
  /*

   [1]
    |
    |
   [0]

  */
  const auto g = create_test_graph_6();
  const sp_vert_desc vd_a = *boost::vertices(g).first;
  sp_vert_desc vd_b = vd_a; ++vd_b;
  BOOST_CHECK(!is_tip(vd_a, g));
  BOOST_CHECK( is_tip(vd_b, g));
}


BOOST_AUTO_TEST_CASE(sado_collect_root_vds_7)
{
  //One root node
  {
    const auto g = create_test_graph_7();
    BOOST_CHECK_EQUAL(collect_root_vds(g).size(), 1);
  }
  //Multiple root nodes
}


BOOST_AUTO_TEST_CASE(sado_create_reconstructed_for_non_merged_phylogenies)
{
  {
    const auto g = create_test_graph_2();
    const auto r = create_reconstructed(g);
    BOOST_CHECK(is_isomorphic(g, r));
  }
  {
    const auto g = create_test_graph_3();
    const auto r = create_reconstructed(g);
    BOOST_CHECK(is_isomorphic(g, r));

  }
  {
    const auto g = create_test_graph_5();
    const auto r = create_reconstructed(g);
    BOOST_CHECK(!is_isomorphic(g, r));
  }
  {
    const auto g = create_test_graph_6();
    const auto r = create_reconstructed(g);
    BOOST_CHECK(is_isomorphic(g, r));
  }
}

BOOST_AUTO_TEST_CASE(sado_create_reconstructed_7)
{
  const auto g = create_test_graph_7();
  const auto r = create_reconstructed(g);
  BOOST_CHECK_EQUAL(boost::num_vertices(g), 1);

  //Do not delete our only species
  BOOST_CHECK_EQUAL(boost::num_vertices(r), 1);
  BOOST_CHECK(is_isomorphic(g, r));
}

BOOST_AUTO_TEST_CASE(sado_create_reconstructed_for_mergable_phylogenies_4)
{
  const auto g = create_test_graph_4();
  const auto r = create_reconstructed(g);
  BOOST_CHECK(!is_isomorphic(g, r));
}

BOOST_AUTO_TEST_CASE(sado_create_reconstructed_for_mergable_phylogenies_8)
{
  const auto g = create_test_graph_8();
  const auto r = create_reconstructed(g);
  BOOST_CHECK(!is_isomorphic(g, r));
}

BOOST_AUTO_TEST_CASE(sado_create_reconstructed_should_always_work)
{
  for (const auto g: create_test_graphs())
  {
    BOOST_CHECK_NO_THROW(create_reconstructed(g));
  }
}

BOOST_AUTO_TEST_CASE(sado_count_number_species_in_generation_3)
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
  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 0), 1);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 1), 1);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 2), 1);
}

BOOST_AUTO_TEST_CASE(sado_count_number_species_in_generation_2)
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
  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 0), 1);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 1), 1);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 2), 2);
}

BOOST_AUTO_TEST_CASE(sado_count_number_species_in_generation_1)
{
  /*

   0 -> .
   1 -> .
   2 -> 0
   3 -> {0, 1}
   4 -> {0, 1}
   5 -> {0, 1}
   6 -> {2, 3}
   7 -> {4, 5}

      [6]      [7]
      /\        /\
     /  \      /  \
   [2]  [3]  [4] [5]
     \   |\  /| ./
      \  | \/.| /
       \ |./\ |/
        [0]  [1]
  */
  const auto g = create_test_graph_1();
  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 0), 2);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 1), 4);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 2), 2);

  const auto h = create_reconstructed(g);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 0), 1);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 1), 2);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 2), 2);
}

BOOST_AUTO_TEST_CASE(sado_merge_two_species)
{
  /*
   [3]          [3]
    | \         |
    |  \        |
   [1]  [2] -> [1]
    |  /        |
    | /         |
   [0]         [0]
  */
  const auto g = create_test_graph_4();
  const auto h = create_reconstructed(g);

  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 0), 1);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 1), 2);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 2), 1);

  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 0), 1);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 1), 1);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 2), 1);
}

BOOST_AUTO_TEST_CASE(sado_merge_two_species_graph_9)
{
  /*
    [5]       [5]
    /\         |
   /  \        |
  [3] [4]     [4]
  |    |       |
  |    |       |
 [1]  [2] ->  [1]
  |   /        |
  |  /         |
  [0]         [0]
  */

  const auto g = create_test_graph_9();
  const auto h = create_reconstructed(g);

  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 0), 1);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 1), 2);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 2), 2);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 3), 1);


  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 0), 1);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 1), 1);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 2), 1);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 3), 1);

}

BOOST_AUTO_TEST_CASE(sado_merge_two_species_graph_10)
{
  /*
 [4]          [4]
  | \          |
  |  \         |
 [2]  [3] ->  [2]
  |  / |       |
  | /  |       |
 [0]  [1]     [0]
  */

  const auto g = create_test_graph_10();
  const auto h = create_reconstructed(g);

  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 0), 2);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 1), 2);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 2), 1);


  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 0), 1);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 1), 1);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 2), 1);

}

BOOST_AUTO_TEST_CASE(sado_merge_two_species_graph_11)
{
  /*
 [4]          [4]
  | \          |
  |  \         |
 [2]  [3] ->  [2]
  | \  |       |
  |  \ |       |
 [0]  [1]     [0]
  */

  const auto g = create_test_graph_11();
  const auto h = create_reconstructed(g);

  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 0), 2);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 1), 2);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 2), 1);


  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 0), 1);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 1), 1);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 2), 1);

}

BOOST_AUTO_TEST_CASE(sado_merge_two_species_graph_12)
{
  /*
     [5]         [5]
    / | \         |
   /  |  \        |
 [2] [3] [4] ->  [2]
    \ | / |       |
     \|/  |       |
     [0] [1]     [0]
  */

  const auto g = create_test_graph_12();
  const auto h = create_reconstructed(g);

  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 0), 2);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 1), 3);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 2), 1);


  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 0), 1);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 1), 1);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 2), 1);

}

BOOST_AUTO_TEST_CASE(sado_merge_two_species_graph_13)
{
  /*

     [3] [4]     [3] [4]
      | / |       | /
      |/  |       |/
     [1] [2] ->  [1,2]
      | /         |
      |/          |
     [0]         [0]
  */

  const auto g = create_test_graph_13();
  const auto h = create_reconstructed(g);

  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 0), 1);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 1), 2);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 2), 2);


  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 0), 1);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 1), 1);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 2), 2);

}

BOOST_AUTO_TEST_CASE(sado_merge_two_species_graph_14)
{
  /*

     [3] [4]     [3] [4]
      |\  |       | /
      | \ |       |/
     [1] [2] ->  [1,2]
      | /         |
      |/          |
     [0]         [0]
  */

  const auto g = create_test_graph_14();
  const auto h = create_reconstructed(g);
  BOOST_CHECK_EQUAL(boost::num_edges(g), 5);
  BOOST_CHECK_EQUAL(boost::num_edges(h), 3);

  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 0), 1);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 1), 2);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 2), 2);

  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 0), 1);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 1), 1);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 2), 2);
}

BOOST_AUTO_TEST_CASE(sado_merge_two_species_graph_15)
{
  /*

     [3]            [3]
      |              |
      |              |
     [2]      ->    [2]
      | \            |
      |  \           |
     [0] [1]        [0]
  */

  const auto g = create_test_graph_15();

  const auto vd_0 = *vertices(g).first;
  auto vd_1 = vd_0; ++vd_1;
  auto vd_2 = vd_1; ++vd_2;
  auto vd_3 = vd_2; ++vd_3;
  assert(g[vd_0].get_generation() == 0);
  assert(g[vd_1].get_generation() == 0);
  assert(has_common_descendant(vd_0, vd_1, g));

  const auto h = create_reconstructed(g);
  BOOST_CHECK_EQUAL(boost::num_edges(g), 3);
  BOOST_CHECK_EQUAL(boost::num_edges(h), 2);

  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 0), 2);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 1), 1);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 2), 1);

  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 0), 1);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 1), 1);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 2), 1);
}

BOOST_AUTO_TEST_CASE(sado_merge_two_species_graph_16)
{
  /*

     [3] [4]     [3] [4]
      |   |       |   |
      |   |       |   |
     [1] [2] ->  [1] [2]
      | /         |  /
      |/          | /
     [0]         [0]
  */

  const auto g = create_test_graph_16();
  const auto h = create_reconstructed(g);

  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 0), 1);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 1), 2);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 2), 2);


  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 0), 1);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 1), 2);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 2), 2);
}


BOOST_AUTO_TEST_CASE(sado_merge_two_species_graph_17)
{
 /*
             [8]
            / |
           /  |
         [6] [7]
        / |   |
       /  |   |
     [3] [4] [5]
      |   |  /
      |   | /
     [1] [2]
      | /
      |/
     [0]
  */

  const auto g = create_test_graph_17();
  const auto h = create_reconstructed(g);

  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 0), 1);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 1), 2);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 2), 3);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 3), 2);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 4), 1);

  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 0), 1);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 1), 1);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 2), 1);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 3), 1);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 4), 1);
}

BOOST_AUTO_TEST_CASE(sado_merge_two_species_graph_20)
{
  /*

      [3]         [2]
     / | \         |
    /  |  \        |
   |  [1]--[2] -> [1]
    \  |  /        |
     \ | /         |
      [0]         [0]

   */

  const auto g = create_test_graph_20();
  const auto h = create_reconstructed(g);

  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 0), 1);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 1), 2);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 2), 1);

  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 0), 1);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 1), 1);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 2), 1);

  BOOST_CHECK_EQUAL(boost::num_edges(g), 6);
  BOOST_CHECK_EQUAL(boost::num_edges(h), 2);
}

BOOST_AUTO_TEST_CASE(sado_merge_two_species_graph_21)
{
  /*

      [2]        [2]
     / |          |
    |  |          |
    | [1]   -->  [1]
    \  |          |
     \ |          |
      [0]        [0]
   */

  const auto g = create_test_graph_21();
  const auto h = create_reconstructed(g);

  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 0), 1);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 1), 1);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(g, 2), 1);

  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 0), 1);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 1), 1);
  BOOST_CHECK_EQUAL(count_number_species_in_generation(h, 2), 1);

  BOOST_CHECK_EQUAL(boost::num_edges(g), 3);
  BOOST_CHECK_EQUAL(boost::num_edges(h), 2);
}

BOOST_AUTO_TEST_CASE(sado_get_next_generation_vds)
{
  {
    /*

     [0]

    */
    //If there is only one generation, there will be no vds in the next generation
    const auto g = create_test_graph_7();
    const auto vip = vertices(g);
    const auto vds = get_next_generation_vds(*vip.first ,g);
    BOOST_CHECK_EQUAL(vds.size(), 0);
  }
  {
    /*
     [1]
      |
      |
     [0]
    */
    //If there is are two generations, there will be
    // * one vd in the oldest generation
    // * zero vd in the youngest generation
    const auto g = create_test_graph_6();
    const auto vip = vertices(g);
    const auto vd_oldest = *vip.first;
    auto vd_youngest = vd_oldest; ++vd_youngest;
    assert(g[vd_oldest].get_generation() == 0);
    assert(g[vd_youngest].get_generation() == 1);
    const auto vds_oldest = get_next_generation_vds(vd_oldest,g);
    const auto vds_youngest = get_next_generation_vds(vd_youngest,g);
    BOOST_CHECK_EQUAL(vds_oldest.size(), 1);
    BOOST_CHECK_EQUAL(vds_youngest.size(), 0);
  }
}

BOOST_AUTO_TEST_CASE(sado_vertices_go_from_ancestor_to_kid)
{
/*

ancestor      kid
   [0] -----> [1]
   vd_a       vd_b

*/

  const auto g = create_test_graph_6();
  assert(boost::num_vertices(g) == 2);
  assert(boost::num_edges(g) == 1);
  const auto vd_a = boost::vertex(0, g);
  const auto vd_b = boost::vertex(1, g);
  BOOST_CHECK_EQUAL(boost::in_degree(vd_a, g), 0);
  BOOST_CHECK_EQUAL(boost::in_degree(vd_b, g), 1);
  BOOST_CHECK_EQUAL(boost::out_degree(vd_a, g), 1);
  BOOST_CHECK_EQUAL(boost::out_degree(vd_b, g), 0);
  BOOST_CHECK(!has_ancestor(vd_a, g));
  BOOST_CHECK( has_ancestor(vd_b, g));
}



#pragma GCC diagnostic pop
