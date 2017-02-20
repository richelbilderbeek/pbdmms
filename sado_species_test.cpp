#include "sado_species.h"
#include "sado_individual.h"
#include "sado_species_graph.h"

#include <fstream>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace sado;

BOOST_AUTO_TEST_CASE(sado_species_test_trivial_functions)
{
  species test_species;
  BOOST_CHECK(test_species.empty());

  const indiv i;
  test_species.add_indiv(i);
  BOOST_CHECK(static_cast<int>(test_species.size()) == 1);
  const indiv j;
  test_species.add_indiv(j);

  BOOST_CHECK(static_cast<int>(test_species.size()) == 2);
  BOOST_CHECK(i != j);
  BOOST_CHECK(test_species[0] != j);
  BOOST_CHECK(test_species[0] == i);
  BOOST_CHECK(test_species[1] == j);
  BOOST_CHECK(test_species[1] != i);
}

BOOST_AUTO_TEST_CASE(species_get_right_gen_number)
{
  species test_species(42);
  BOOST_CHECK_EQUAL(test_species.get_generation(), 42);
}

BOOST_AUTO_TEST_CASE(sado_has_extant_descendant_simplest)
{
  /*

   [0] //Has no descendants

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
  BOOST_CHECK_EQUAL(n, 0);
}

BOOST_AUTO_TEST_CASE(sado_has_extant_descendant_simpler)
{
  /*

   [1] //Has no descendants
    |
    |
   [0] //Has one descendant
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
  BOOST_CHECK_EQUAL(n, 1);

}





BOOST_AUTO_TEST_CASE(sado_has_extant_descendant_simple)
{
  /*
   [2] //Has no descendants
    |
    |
   [1] //Has one descendant
    |
    |
   [0] //Has two descendants
  */
  const auto g = create_test_graph_2();
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
  //Three vertices do have a descendant that is extant (0, 1 and 3)
  //One vertex, 2, does not
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
  BOOST_CHECK_EQUAL(n, 3);
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
    const auto n = std::count_if(
      vip.first,
      vip.second,
      [g](const sp_vert_desc vd)
      {
        const int expected{1};
        return get_last_descendant_generation(vd, g) == expected;
      }
    );

    BOOST_CHECK_EQUAL(n, boost::num_vertices(g));
  }


}



#pragma GCC diagnostic pop
