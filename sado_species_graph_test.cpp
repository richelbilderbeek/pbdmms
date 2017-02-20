#include "sado_species_graph.h"
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
  save_graph_to_dot(g, "test_graph_1.dot");
  convert_dot_to_svg("test_graph_1.dot", "test_graph_1.svg");
  convert_svg_to_png("test_graph_1.svg","test_graph_1.png");
}

BOOST_AUTO_TEST_CASE(sado_create_reconstructed_graph_from_species_graph_for_non_merged_phylogenies)
{
  {
    const auto g = create_test_graph_2();
    const auto r = create_reconstructed_graph_from_species_graph(g);
    BOOST_CHECK(is_isomorphic(g, r));
  }
  {
    const auto g = create_test_graph_3();
    const auto r = create_reconstructed_graph_from_species_graph(g);
    BOOST_CHECK(is_isomorphic(g, r));
  }
  {
    const auto g = create_test_graph_5();
    const auto r = create_reconstructed_graph_from_species_graph(g);
    BOOST_CHECK(!is_isomorphic(g, r));
  }
  {
    const auto g = create_test_graph_6();
    const auto r = create_reconstructed_graph_from_species_graph(g);
    BOOST_CHECK(is_isomorphic(g, r));
  }
  {
    const auto g = create_test_graph_7();
    const auto r = create_reconstructed_graph_from_species_graph(g);
    BOOST_CHECK(is_isomorphic(g, r));
  }
}

BOOST_AUTO_TEST_CASE(sado_create_reconstructed_graph_from_species_graph_for_mergable_phylogenies)
{
  const auto g = create_test_graph_4();
  const auto r = create_reconstructed_graph_from_species_graph(g);
  BOOST_CHECK(!is_isomorphic(g, r));
}

BOOST_AUTO_TEST_CASE(sado_count_number_reconstructed_species_in_generation_3)
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
  BOOST_CHECK_EQUAL(count_number_reconstructed_species_in_generation(g, 0), 1);
  BOOST_CHECK_EQUAL(count_number_reconstructed_species_in_generation(g, 1), 1);
  BOOST_CHECK_EQUAL(count_number_reconstructed_species_in_generation(g, 2), 1);
}

BOOST_AUTO_TEST_CASE(sado_count_number_reconstructed_species_in_generation_2)
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
  BOOST_CHECK_EQUAL(count_number_reconstructed_species_in_generation(g, 0), 1);
  BOOST_CHECK_EQUAL(count_number_reconstructed_species_in_generation(g, 1), 1);
  BOOST_CHECK_EQUAL(count_number_reconstructed_species_in_generation(g, 2), 2);
}

#ifdef sado_merge_two_species_WORKS
//TODO
BOOST_AUTO_TEST_CASE(sado_count_number_reconstructed_species_in_generation_1)
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
  BOOST_CHECK_EQUAL(count_number_reconstructed_species_in_generation(g, 0), 1);
  BOOST_CHECK_EQUAL(count_number_reconstructed_species_in_generation(g, 1), 2);
  BOOST_CHECK_EQUAL(count_number_reconstructed_species_in_generation(g, 2), 2);
}
#endif

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

  BOOST_CHECK_EQUAL(count_number_reconstructed_species_in_generation(g, 0), 1);
  BOOST_CHECK_EQUAL(count_number_reconstructed_species_in_generation(g, 1), 1);
  BOOST_CHECK_EQUAL(count_number_reconstructed_species_in_generation(g, 2), 1);



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



#pragma GCC diagnostic pop
