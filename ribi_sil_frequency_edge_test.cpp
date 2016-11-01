#include "ribi_sil_frequency_edge.h"

#include "ribi_sil_frequency_vertex.h"
#include "find_first_bundled_vertex_with_my_vertex.h"
#include "ribi_sil_frequency_phylogeny.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace ribi;

BOOST_AUTO_TEST_CASE(test_ribi_sil_frequency_edge_use)
{
  const int n_timesteps{42};
  sil_frequency_edge e(n_timesteps);
  BOOST_CHECK_EQUAL(e.get_n_timesteps(), n_timesteps);
}

BOOST_AUTO_TEST_CASE(test_ribi_sil_frequency_edge_abuse)
{
  BOOST_CHECK_THROW(sil_frequency_edge(-42), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(test_ribi_move_sil_connections_1)
{
  /*
             2
            / \
   Past 0--1   4--5 Present
            \ /
             3

   --+---+-------------------
   # | t | fs (SIL + f)
   --+---+-------------------
   0 | 1 | {{000,2}}
   1 | 2 | {{001,2}}
   2 | 3 | {{010,1}}
   3 | 3 | {{011,1}}
   4 | 4 | {{100,2}}
   5 | 5 | {{101,2}}

   should become

             2
           // \\
   Past 0--1   4--5 Present

             3

  */
  auto g = get_test_sil_frequency_phylogeny_1();

  //Find from, which in 3
  const sil sil_from = create_sil("011");
  const std::map<sil,int> sfm_from = { {sil_from, 1} };
  const sil_frequency_vertex v_from(sfm_from, 3);
  const auto vd_from = find_first_bundled_vertex_with_my_vertex(v_from, g);
  assert(boost::degree(vd_from, g) == 2);

  //Find to, which in 2
  const sil sil_to = create_sil("010");
  const std::map<sil,int> sfm_to = { {sil_to, 1} };
  const sil_frequency_vertex v_to(sfm_to, 3);
  const auto vd_to = find_first_bundled_vertex_with_my_vertex(v_to, g);
  assert(boost::degree(vd_to, g) == 2);

  //Move connections from 'from' to 'to'
  move_sil_connection(vd_from, vd_to, g);

  BOOST_CHECK_EQUAL(boost::degree(vd_from, g), 0);
  BOOST_CHECK_EQUAL(boost::degree(vd_to, g), 2);
}

BOOST_AUTO_TEST_CASE(test_ribi_move_sil_connections_2)
{
  /*
             2--4
            /    \
   Past 0--1      6--7 Present
            \    /
             3--5

   --+---+-------------------
   # | t | fs (SIL + f)
   --+---+-------------------
   0 | 1 | {{000,2}}
   1 | 2 | {{001,2}}
   2 | 3 | {{010,1}}
   3 | 3 | {{011,1}}
   4 | 4 | {{010,1}}
   5 | 4 | {{011,1}}
   6 | 5 | {{100,2}}
   7 | 6 | {{101,2}}

   should become

             2==4
           //    \\
   Past 0--1      6--7 Present

             3  5

  */

  auto g = get_test_sil_frequency_phylogeny_2();

  {
    //Find from, which in 5
    const sil sil_from = create_sil("011");
    const std::map<sil,int> sfm_from = { {sil_from, 1} };
    const sil_frequency_vertex v_from(sfm_from, 4);
    const auto vd_from = find_first_bundled_vertex_with_my_vertex(v_from, g);
    assert(boost::degree(vd_from, g) == 2);

    //Find to, which in 4
    const sil sil_to = create_sil("010");
    const std::map<sil,int> sfm_to = { {sil_to, 1} };
    const sil_frequency_vertex v_to(sfm_to, 4);
    const auto vd_to = find_first_bundled_vertex_with_my_vertex(v_to, g);
    assert(boost::degree(vd_to, g) == 2);

    //Move connections from 'from' to 'to'
    move_sil_connection(vd_from, vd_to, g);

    BOOST_CHECK_EQUAL(boost::degree(vd_from, g), 0);
    BOOST_CHECK_EQUAL(boost::degree(vd_to, g), 2);
  }
  /* Intermediate stage:

             2==4
            /    \\
   Past 0--1      6--7 Present
            \
             3  5

  */
  {
    //Find from, which in 3
    const sil sil_from = create_sil("011");
    const std::map<sil,int> sfm_from = { {sil_from, 1} };
    const sil_frequency_vertex v_from(sfm_from, 3);
    const auto vd_from = find_first_bundled_vertex_with_my_vertex(v_from, g);
    BOOST_CHECK_EQUAL(boost::degree(vd_from, g), 1);

    //Find to, which in 2
    const sil sil_to = create_sil("010");
    const std::map<sil,int> sfm_to = { {sil_to, 1} };
    const sil_frequency_vertex v_to(sfm_to, 3);
    const auto vd_to = find_first_bundled_vertex_with_my_vertex(v_to, g);
    BOOST_CHECK_EQUAL(boost::degree(vd_to, g), 2);

    //Move connections from 'from' to 'to'
    move_sil_connection(vd_from, vd_to, g);

    BOOST_CHECK_EQUAL(boost::degree(vd_from, g), 0);
    BOOST_CHECK_EQUAL(boost::degree(vd_to, g), 2);
  }
}

#pragma GCC diagnostic pop

