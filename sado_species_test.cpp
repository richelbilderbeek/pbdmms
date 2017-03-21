#include "sado_species.h"
#include "sado_individual.h"
#include "sado_ancestry_graph.h"

#include <fstream>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace sado;

BOOST_AUTO_TEST_CASE(sado_species_construction)
{
  const int generation{42};
  const indiv i;
  const indiv j;
  const species s(generation, {i, j} );
  BOOST_CHECK_EQUAL(s.size(), 2);
  BOOST_CHECK(s.has_individual(i));
  BOOST_CHECK(s.has_individual(j));
  BOOST_CHECK_EQUAL(s.get_generation(), generation);
}

BOOST_AUTO_TEST_CASE(sado_has_extant_descendant_simplest)
{
  /*

   [0] //Has extant descendant

  */
  const auto g = create_test_graph_7();
  const auto vip = vertices(g);
  const int n =
    std::count_if(
      vip.first,
      vip.second,
      [g](const auto vi)
      {
        return has_extant_descendant(vi, g);
      }
    )
  ;
  BOOST_CHECK_EQUAL(n, 1);
}

BOOST_AUTO_TEST_CASE(sado_has_extant_descendant_simpler)
{
  /*

   [1] //Has extant descendant
    |
    |
   [0] //Has extant descendant
  */

  const auto g = create_test_graph_6();
  const auto vip = vertices(g);
  const int n =
    std::count_if(
      vip.first,
      vip.second,
      [g](const auto vi)
      {
        return has_extant_descendant(vi, g);
      }
    )
  ;
  BOOST_CHECK_EQUAL(n, 2);

}





BOOST_AUTO_TEST_CASE(sado_has_extant_descendant_simple)
{
  /*
   [2] //Has extant descendant
    |
    |
   [1] //Has extant descendant
    |
    |
   [0] //Has extant descendant
  */
  const auto g = create_test_graph_3();
  const auto vip = vertices(g);
  const auto vd_1 = *vip.first;
  auto vd_2 = vd_1; ++vd_2;
  auto vd_3 = vd_2; ++vd_3;
  assert(g[vd_1].get_generation() == 0);
  assert(g[vd_2].get_generation() == 1);
  assert(g[vd_3].get_generation() == 2);

  BOOST_CHECK(has_extant_descendant(vd_1,g));
  BOOST_CHECK(has_extant_descendant(vd_2,g));
  BOOST_CHECK(has_extant_descendant(vd_3,g));

}

BOOST_AUTO_TEST_CASE(sado_has_multiple_extant_descendants)
{
  const auto g = create_test_graph_2();

  /*
   [2]  [3]
    |  /
    | /
   [1]
    |
    |
   [0]
  */

  const auto vip = vertices(g);

  const auto vd_1 = *vip.first;
  auto vd_2 = vd_1; ++vd_2;
  auto vd_3 = vd_2; ++vd_3;
  auto vd_4 = vd_3; ++vd_4;
  assert(g[vd_1].get_generation() == 0);
  assert(g[vd_2].get_generation() == 1);
  assert(g[vd_3].get_generation() == 2);
  assert(g[vd_4].get_generation() == 2);

  BOOST_CHECK( has_extant_descendant(vd_1,g));
  BOOST_CHECK( has_extant_descendant(vd_2,g));
  BOOST_CHECK( has_extant_descendant(vd_3,g));
  BOOST_CHECK( has_extant_descendant(vd_4,g));
}


BOOST_AUTO_TEST_CASE(sado_has_extant_descendant)
{
  const auto g = create_test_graph_5();
  /*
   [3]
    |
    |
   [1]  [2]
    |  /
    | /
   [0]
  */
  //Three vertices do have an extant descendant that is extant (0, 1 and 3)
  //One vertex, 2, does not

  const auto vip = vertices(g);

  const auto vd_1 = *vip.first;
  auto vd_2 = vd_1; ++vd_2;
  auto vd_3 = vd_2; ++vd_3;
  auto vd_4 = vd_3; ++vd_4;
  assert(g[vd_1].get_generation() == 0);
  assert(g[vd_2].get_generation() == 1);
  assert(g[vd_3].get_generation() == 1);
  assert(g[vd_4].get_generation() == 2);

  //Check that [2] is the extinct species, by verifying
  //that [3] is the offspring of [1]

  assert(
    std::begin(g[vd_4].get_indivs())->get_father_id() ==
    std::begin(g[vd_2].get_indivs())->get_id()
  );
  //assert(spp[3][0].get_father_id() == spp[1][0].get_id());


  BOOST_CHECK( has_extant_descendant(vd_1,g));
  BOOST_CHECK( has_extant_descendant(vd_2,g));
  BOOST_CHECK(!has_extant_descendant(vd_3,g));
  BOOST_CHECK( has_extant_descendant(vd_4,g));

}

BOOST_AUTO_TEST_CASE(sado_count_number_generations)
{
const auto g = create_test_graph_6();
BOOST_CHECK_EQUAL(count_n_generations(g), 2);

const auto h = create_test_graph_7();
BOOST_CHECK_EQUAL(count_n_generations(h), 1);

const auto i = create_test_graph_5();
BOOST_CHECK_EQUAL(count_n_generations(i), 3);

}


BOOST_AUTO_TEST_CASE(sado_get_last_descendant_generation)
{
  {
    /*

     [0]

    */
    const auto g = create_test_graph_7();
    //Each individual should have '1' as its last generations
    const auto vip = vertices(g);
    assert(boost::num_vertices(g) == 1);
    BOOST_CHECK_EQUAL(get_last_descendant_generation(*vip.first, g), 0);
  }

  {
    /*
     [1]
      |
      |
     [0]
    */
    const auto g = create_test_graph_6();
    //Each individual should have '1' as its last generations
    const auto vip = vertices(g);

    const auto vd_1 = *vip.first;
    auto vd_2 = vd_1; ++vd_2;
    assert(g[vd_1].get_generation() == 0);
    assert(g[vd_2].get_generation() == 1);

    BOOST_CHECK_EQUAL(get_last_descendant_generation(vd_1,g),1);
    BOOST_CHECK_EQUAL(get_last_descendant_generation(vd_2,g),1);
  }


}

BOOST_AUTO_TEST_CASE(sado_get_descendants_7)
{
  /*

   [0]

  */
  const auto g = create_test_graph_7();
  //Each individual should have '1' as its last generations
  const auto vip = vertices(g);
  assert(boost::num_vertices(g) == 1);
  BOOST_CHECK_EQUAL(get_descendants(*vip.first, g).size(), 0);
}

BOOST_AUTO_TEST_CASE(sado_get_descendants_6)
{
  /*
   [1]
    |
    |
   [0]
  */
  const auto g = create_test_graph_6();
  //Each individual should have '1' as its last generations
  const auto vip = vertices(g);

  const auto vd_oldest = *vip.first;
  auto vd_youngest = vd_oldest; ++vd_youngest;
  assert(g[vd_oldest].get_generation() == 0);
  assert(g[vd_youngest].get_generation() == 1);

  BOOST_CHECK_EQUAL(get_descendants(vd_oldest,g).size(), 1);
  BOOST_CHECK_EQUAL(get_descendants(vd_youngest,g).size(), 0);
}




#pragma GCC diagnostic pop
