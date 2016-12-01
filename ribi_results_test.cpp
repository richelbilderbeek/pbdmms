#include "ribi_results.h"
#include <fstream>
#include <boost/graph/adjacency_list.hpp>
#include "ribi_population_factory.h"
#include "is_regular_file.h"
#include "convert_dot_to_svg.h"
#include "convert_svg_to_png.h"
#include "is_isomorphic.h"
#include "ribi_sil_frequency_vertex.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace ribi;



BOOST_AUTO_TEST_CASE(ribi_find_first_with_sil_abuse)
{
  sil_frequency_phylogeny g;
  sil s;
  BOOST_CHECK_THROW(find_first_with_sil(s, g), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(ribi_clear_vertex_with_id_use)
{
  sil_frequency_phylogeny g;
  const auto vd1 = boost::add_vertex(g);
  const auto vd2 = boost::add_vertex(g);
  const std::map<sil,int> sil_frequencies;
  const sil_frequency_vertex v1(sil_frequencies, 42);
  const sil_frequency_vertex v2(sil_frequencies, 42);
  g[vd1] = v1;
  g[vd2] = v2;
  add_sil_frequency_edge(sil_frequency_edge(), vd1, vd2, g);

  BOOST_CHECK_EQUAL(boost::num_edges(g), 1);
  BOOST_CHECK_NO_THROW(clear_vertex_with_id(v1.get_id(), g));
  BOOST_CHECK_EQUAL(boost::num_edges(g), 0);

}

BOOST_AUTO_TEST_CASE(ribi_clear_vertex_with_id_abuse)
{
  sil_frequency_phylogeny g;
  BOOST_CHECK_THROW(clear_vertex_with_id(42, g), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(ribi_all_vds_have_same_time_for_empty_graph)
{
  const sil_frequency_phylogeny g;
  const std::vector<sil_frequency_vertex_descriptor> vds;
  BOOST_CHECK(all_vds_have_same_time(vds, g));
}

BOOST_AUTO_TEST_CASE(ribi_all_vds_have_same_time_for_graph_with_two_nodes_of_same_time)
{
  sil_frequency_phylogeny g;
  const std::map<sil,int> sil_frequencies;
  const auto vd1 = boost::add_vertex(g);
  const auto vd2 = boost::add_vertex(g);
  g[vd1] = sil_frequency_vertex(sil_frequencies, 42);
  g[vd2] = sil_frequency_vertex(sil_frequencies, 42);
  const std::vector<sil_frequency_vertex_descriptor> vds = { vd1, vd2 };
  BOOST_CHECK(all_vds_have_same_time(vds, g));
}

BOOST_AUTO_TEST_CASE(ribi_all_vds_have_same_time_for_graph_with_two_nodes_of_different_time)
{
  sil_frequency_phylogeny g;
  const std::map<sil,int> sil_frequencies;
  const auto vd1 = boost::add_vertex(g);
  const auto vd2 = boost::add_vertex(g);
  g[vd1] = sil_frequency_vertex(sil_frequencies, 314);
  g[vd2] = sil_frequency_vertex(sil_frequencies, 42);
  const std::vector<sil_frequency_vertex_descriptor> vds = { vd1, vd2 };
  BOOST_CHECK(!all_vds_have_same_time(vds, g));
}

BOOST_AUTO_TEST_CASE(ribi_all_zero_vds_have_same_id)
{
  const sil_frequency_phylogeny g;
  const std::vector<sil_frequency_vertex_descriptor> vds;
  BOOST_CHECK(all_vds_have_same_id(vds, g));
}


BOOST_AUTO_TEST_CASE(ribi_all_one_vds_have_same_id)
{
  sil_frequency_phylogeny g;
  const std::map<sil,int> sils = { {create_sil("01010"), 1} };
  const std::vector<sil_frequency_vertex_descriptor> vds
    = add_sils(sils, 0, g)
  ;
  BOOST_CHECK(all_vds_have_same_id(vds, g));
}

BOOST_AUTO_TEST_CASE(ribi_summarize_genotypes)
{
  /*

  Vertices 0 and 1 share the same point in time,
  thus their genotype frequencies can be merged

  0--2--3
  |
  1

  Iin the simulation, there is a connection between 1 and 2,
  as same species are connected. This is tested in case
  'test_ribi_summarize_genotypes_with_extra_connection'

   --+---+-------------------
   # | t | fs (SIL + f)
   --+---+-------------------
   0 | 1 | {{000,1}}
   1 | 1 | {{001,1}}
   2 | 2 | {{010,2}}
   3 | 3 | {{011,2}}
   --+---+-------------------

   Should become

   0--2--3

   --+---+-------------------
   # | t | fs (SIL + f)
   --+---+-------------------
   0 | 1 | {{000,1}, {001,1}}
   2 | 2 | {{010,2}}
   3 | 3 | {{011,2}}
   --+---+-------------------

  */
  sil_frequency_phylogeny g;
  const sil sil0{create_sil("000")};
  const sil sil1{create_sil("001")};
  const sil sil2{create_sil("010")};
  const sil sil3{create_sil("011")};
  const std::map<sil,int> sfs0 = {{sil0, 1}};
  const std::map<sil,int> sfs1 = {{sil1, 1}};
  const std::map<sil,int> sfs2 = {{sil2, 2}};
  const std::map<sil,int> sfs3 = {{sil3, 2}};
  const auto vd0 = boost::add_vertex(sil_frequency_vertex(sfs0, 1),g);
  const auto vd1 = boost::add_vertex(sil_frequency_vertex(sfs1, 1),g);
  const auto vd2 = boost::add_vertex(sil_frequency_vertex(sfs2, 2),g);
  const auto vd3 = boost::add_vertex(sil_frequency_vertex(sfs3, 3),g);
  add_sil_frequency_edge(sil_frequency_edge(1), vd0, vd1, g);
  add_sil_frequency_edge(sil_frequency_edge(1), vd0, vd2, g);
  add_sil_frequency_edge(sil_frequency_edge(1), vd2, vd3, g);

  assert(boost::num_vertices(g) == 4);
  assert(boost::num_edges(g) ==  3);
  summarize_genotypes(g);
  BOOST_CHECK_EQUAL(boost::num_vertices(g), 3);
  BOOST_CHECK_EQUAL(boost::num_edges(g), 2);
}

BOOST_AUTO_TEST_CASE(ribi_summarize_genotypes_with_extra_connection)
{
  /*

  Vertices 0 and 1 share the same point in time,
  thus their genotype frequencies can be merged

  0--2--3
  | /
  |/
  1

  The function should also work when there is no connection
  between 1 and 2. This is tested in 'test_ribi_summarize_genotypes'

   --+---+-------------------
   # | t | fs (SIL + f)
   --+---+-------------------
   0 | 1 | {{000,1}}
   1 | 1 | {{001,1}}
   2 | 2 | {{010,2}}
   3 | 3 | {{011,2}}
   --+---+-------------------

   Should become

   0--2--3

   --+---+-------------------
   # | t | fs (SIL + f)
   --+---+-------------------
   0 | 1 | {{000,1}, {001,1}}
   2 | 2 | {{010,2}}
   3 | 3 | {{011,2}}
   --+---+-------------------

  */
  sil_frequency_phylogeny g;
  const sil sil0{create_sil("000")};
  const sil sil1{create_sil("001")};
  const sil sil2{create_sil("010")};
  const sil sil3{create_sil("011")};
  const std::map<sil,int> sfs0 = {{sil0, 1}};
  const std::map<sil,int> sfs1 = {{sil1, 1}};
  const std::map<sil,int> sfs2 = {{sil2, 2}};
  const std::map<sil,int> sfs3 = {{sil3, 2}};
  const auto vd0 = boost::add_vertex(sil_frequency_vertex(sfs0, 1),g);
  const auto vd1 = boost::add_vertex(sil_frequency_vertex(sfs1, 1),g);
  const auto vd2 = boost::add_vertex(sil_frequency_vertex(sfs2, 2),g);
  const auto vd3 = boost::add_vertex(sil_frequency_vertex(sfs3, 3),g);
  add_sil_frequency_edge(sil_frequency_edge(1), vd0, vd1, g);
  add_sil_frequency_edge(sil_frequency_edge(1), vd0, vd2, g);
  add_sil_frequency_edge(sil_frequency_edge(1), vd1, vd2, g);
  add_sil_frequency_edge(sil_frequency_edge(1), vd2, vd3, g);
  assert(boost::num_vertices(g) == 4);
  assert(boost::num_edges(g) ==  4);
  summarize_genotypes(g);
  BOOST_CHECK_EQUAL(boost::num_vertices(g), 3);
  BOOST_CHECK_EQUAL(boost::num_edges(g), 2);
}


BOOST_AUTO_TEST_CASE(ribi_fuse_vertices_with_same_sil_frequencies_linear_one_style)
{
  /*

     1   1
   0---1---2

   --+---+-------------------
   # | t | fs (SIL + f)
   --+---+-------------------
   0 | 1 | {{000,1}}
   1 | 2 | {{000,1}}
   2 | 3 | {{001,1}}
   --+---+-------------------

   Should become

     2
   0---2 and 1

   --+---+-------------------
   # | t | fs (SIL + f)
   --+---+-------------------
   0 | 1 | {{000,1}}
   1 | - | {}
   2 | 3 | {{001,1}}
   --+---+-------------------

  */
  sil_frequency_phylogeny g;
  const sil sil0{create_sil("000")};
  const sil sil1{create_sil("001")};
  const std::map<sil,int> sfs0 = {{sil0, 1}};
  const std::map<sil,int> sfs1 = {{sil0, 1}};
  const std::map<sil,int> sfs2 = {{sil1, 1}};
  const auto vd0 = boost::add_vertex(sil_frequency_vertex(sfs0, 1),g);
  const auto vd1 = boost::add_vertex(sil_frequency_vertex(sfs1, 2),g);
  const auto vd2 = boost::add_vertex(sil_frequency_vertex(sfs2, 3),g);
  add_sil_frequency_edge(sil_frequency_edge(1), vd0, vd1, g);
  add_sil_frequency_edge(sil_frequency_edge(1), vd1, vd2, g);
  assert(boost::num_vertices(g) == 3);
  assert(boost::num_edges(g) ==  2);
  fuse_vertices_with_same_sil_frequencies(g);
  BOOST_CHECK_EQUAL(boost::num_vertices(g), 2);
  BOOST_CHECK_EQUAL(boost::num_edges(g), 1);
}

BOOST_AUTO_TEST_CASE(ribi_fuse_vertices_with_same_sil_frequencies_linear_two_styles)
{
  /*

   In this example, the fuse_vertices_with_same_sil_frequencies
   should still work, even though the third vertex has a different style:

     1   1
   0---1---2
   G   G   I

   --+---+-------------------
   # | t | fs (SIL + f)
   --+---+-------------------
   0 | 1 | {{000,3}}
   1 | 2 | {{000,3}}
   2 | 3 | {{000,1},{001,1},{011,1}}
   --+---+-------------------

   Should become

     2
   0---2 and 1

   --+---+-------------------
   # | t | fs (SIL + f)
   --+---+-------------------
   0 | 1 | {{000,1}}
   1 | - | {}
   2 | 3 | {{000,1},{001,1},{011,1}}
   --+---+-------------------

  */
  sil_frequency_phylogeny g;
  const sil sil0{create_sil("000")};
  const sil sil1{create_sil("001")};
  const sil sil2{create_sil("011")};
  const std::map<sil,int> sfs0 = {{sil0, 3}};
  const std::map<sil,int> sfs1 = {{sil0, 3}};
  const std::map<sil,int> sfs2 = {{sil0, 1},{sil1, 1},{sil2, 1}};
  const auto sfv0 = sil_frequency_vertex(sfs0, 1);
  const auto sfv1 = sil_frequency_vertex(sfs1, 2);
  const auto sfv2 = sil_frequency_vertex(sfs2, 3);
  const auto vd0 = boost::add_vertex(sfv0,g);
  const auto vd1 = boost::add_vertex(sfv1,g);
  const auto vd2 = boost::add_vertex(sfv2,g);
  add_sil_frequency_edge(sil_frequency_edge(1), vd0, vd1, g);
  add_sil_frequency_edge(sil_frequency_edge(1), vd1, vd2, g);
  assert(boost::num_vertices(g) == 3);
  assert(boost::num_edges(g) ==  2);
  fuse_vertices_with_same_sil_frequencies(g);
  BOOST_CHECK_EQUAL(boost::num_vertices(g), 2);
  BOOST_CHECK_EQUAL(boost::num_edges(g), 1);
}

BOOST_AUTO_TEST_CASE(ribi_fuse_vertices_with_same_sil_frequencies_linear_multiple_styles)
{
  /*

     1   1   1   1   1
   0---1---2---3---4---5
   G   G   I   I   G   G

   --+---+-------------------
   # | t | fs (SIL + f)
   --+---+-------------------
   0 | 1 | {{000,3}}
   1 | 2 | {{000,3}}
   2 | 3 | {{000,1},{001,1},{011,1}}
   3 | 4 | {{000,1},{001,1},{011,1}}
   4 | 5 | {{000,3}}
   5 | 6 | {{000,3}}
   --+---+-------------------

   Should become

     2   2   1
   0---2---4---5
   G   I   G   G

   --+---+-------------------
   # | t | fs (SIL + f)
   --+---+-------------------
   0 | 1 | {{000,3}}
   2 | 3 | {{000,1},{001,1},{011,1}}
   4 | 5 | {{000,3}}
   5 | 6 | {{000,3}}
   --+---+-------------------

  */
  sil_frequency_phylogeny g;
  const sil sil0{create_sil("000")};
  const sil sil1{create_sil("001")};
  const sil sil2{create_sil("011")};
  const std::map<sil,int> sfs0 = {{sil0, 3}};
  const std::map<sil,int> sfs1 = {{sil0, 3}};
  const std::map<sil,int> sfs2 = {{sil0, 1},{sil1, 1},{sil2, 1}};
  const std::map<sil,int> sfs3 = {{sil0, 1},{sil1, 1},{sil2, 1}};
  const std::map<sil,int> sfs4 = {{sil0, 3}};
  const std::map<sil,int> sfs5 = {{sil0, 3}};
  const auto sfv0 = sil_frequency_vertex(sfs0, 1);
  const auto sfv1 = sil_frequency_vertex(sfs1, 2);
  const auto sfv2 = sil_frequency_vertex(sfs2, 3);
  const auto sfv3 = sil_frequency_vertex(sfs3, 4);
  const auto sfv4 = sil_frequency_vertex(sfs4, 5);
  const auto sfv5 = sil_frequency_vertex(sfs5, 6);
  const auto vd0 = boost::add_vertex(sfv0,g);
  const auto vd1 = boost::add_vertex(sfv1,g);
  const auto vd2 = boost::add_vertex(sfv2,g);
  const auto vd3 = boost::add_vertex(sfv3,g);
  const auto vd4 = boost::add_vertex(sfv4,g);
  const auto vd5 = boost::add_vertex(sfv5,g);
  add_sil_frequency_edge(sil_frequency_edge(1), vd0, vd1, g);
  add_sil_frequency_edge(sil_frequency_edge(1), vd1, vd2, g);
  add_sil_frequency_edge(sil_frequency_edge(1), vd2, vd3, g);
  add_sil_frequency_edge(sil_frequency_edge(1), vd3, vd4, g);
  add_sil_frequency_edge(sil_frequency_edge(1), vd4, vd5, g);
  assert(boost::num_vertices(g) == 6);
  assert(boost::num_edges(g) ==  5);
  fuse_vertices_with_same_sil_frequencies(g);
  BOOST_CHECK_EQUAL(boost::num_vertices(g), 4);
  BOOST_CHECK_EQUAL(boost::num_edges(g), 3);
}

BOOST_AUTO_TEST_CASE(ribi_fuse_vertices_with_same_sil_frequencies_fork_of_one)
{
  /*

  This SIL frequency graph should be left alone:

     1   1
   0---1---2
       | 1
       +---3

   --+---+-------------------
   # | t | fs (SIL + f)
   --+---+-------------------
   0 | 1 | {{000,2}}
   1 | 2 | {{000,1},{001,1}}
   2 | 3 | {{000,1}
   3 | 3 | {{001,1}
   --+---+-------------------

  */
  sil_frequency_phylogeny g;
  const sil sil0{create_sil("000")};
  const sil sil1{create_sil("001")};
  const std::map<sil,int> sfs0 = {{sil0, 2}};
  const std::map<sil,int> sfs1 = {{sil0, 1}, {sil1, 1}};
  const std::map<sil,int> sfs2 = {{sil0, 1}};
  const std::map<sil,int> sfs3 = {{sil1, 1}};
  const auto sfv0 = sil_frequency_vertex(sfs0, 1);
  const auto sfv1 = sil_frequency_vertex(sfs1, 2);
  const auto sfv2 = sil_frequency_vertex(sfs2, 3);
  const auto sfv3 = sil_frequency_vertex(sfs3, 3);
  const auto vd0 = boost::add_vertex(sfv0,g);
  const auto vd1 = boost::add_vertex(sfv1,g);
  const auto vd2 = boost::add_vertex(sfv2,g);
  const auto vd3 = boost::add_vertex(sfv3,g);
  add_sil_frequency_edge(sil_frequency_edge(1), vd0, vd1, g);
  add_sil_frequency_edge(sil_frequency_edge(1), vd1, vd2, g);
  add_sil_frequency_edge(sil_frequency_edge(1), vd1, vd3, g);
  assert(boost::num_vertices(g) == 4);
  assert(boost::num_edges(g) ==  3);
  fuse_vertices_with_same_sil_frequencies(g);
  BOOST_CHECK_EQUAL(boost::num_vertices(g), 4);
  BOOST_CHECK_EQUAL(boost::num_edges(g), 3);
}

BOOST_AUTO_TEST_CASE(ribi_fuse_vertices_with_same_sil_frequencies_fork_of_two)
{
  /*

     1   1   1
   0---1---2---4
       | 1   1
       +---3---5

   --+---+-------------------
   # | t | fs (SIL + f)
   --+---+-------------------
   0 | 1 | {{000,2}}
   1 | 2 | {{000,1},{001,1}}
   2 | 3 | {{000,1}
   3 | 3 | {{001,1}
   4 | 4 | {{000,1}
   5 | 4 | {{001,1}
   --+---+-------------------

  Should remain the same, because 1 has different SILs

     1   1   1
   0---1---2---4
       | 1   1
       +---3---5

  Would 1 be a good species, then it should change to

     1   2
   0---1---4
       | 2
       +---5

   --+---+-------------------
   # | t | fs (SIL + f)
   --+---+-------------------
   0 | 1 | {{000,2}}
   1 | 2 | {{000,1},{001,1}}
   2 | 3 | {{000,1}
   3 | 3 | {{001,1}
   --+---+-------------------


  */
  sil_frequency_phylogeny g;
  const sil sil0{create_sil("000")};
  const sil sil1{create_sil("001")};
  const std::map<sil,int> sfs0 = {{sil0, 2}};
  const std::map<sil,int> sfs1 = {{sil0, 1}, {sil1, 1}};
  const std::map<sil,int> sfs2 = {{sil0, 1}};
  const std::map<sil,int> sfs3 = {{sil1, 1}};
  const std::map<sil,int> sfs4 = {{sil0, 1}};
  const std::map<sil,int> sfs5 = {{sil1, 1}};
  const auto sfv0 = sil_frequency_vertex(sfs0, 1);
  const auto sfv1 = sil_frequency_vertex(sfs1, 2);
  const auto sfv2 = sil_frequency_vertex(sfs2, 3);
  const auto sfv3 = sil_frequency_vertex(sfs3, 3);
  const auto sfv4 = sil_frequency_vertex(sfs2, 4);
  const auto sfv5 = sil_frequency_vertex(sfs3, 4);
  const auto vd0 = boost::add_vertex(sfv0,g);
  const auto vd1 = boost::add_vertex(sfv1,g);
  const auto vd2 = boost::add_vertex(sfv2,g);
  const auto vd3 = boost::add_vertex(sfv3,g);
  const auto vd4 = boost::add_vertex(sfv4,g);
  const auto vd5 = boost::add_vertex(sfv5,g);
  add_sil_frequency_edge(sil_frequency_edge(1), vd0, vd1, g);
  add_sil_frequency_edge(sil_frequency_edge(1), vd1, vd2, g);
  add_sil_frequency_edge(sil_frequency_edge(1), vd1, vd3, g);
  add_sil_frequency_edge(sil_frequency_edge(1), vd2, vd4, g);
  add_sil_frequency_edge(sil_frequency_edge(1), vd3, vd5, g);
  assert(boost::num_vertices(g) == 6);
  assert(boost::num_edges(g) ==  5);
  fuse_vertices_with_same_sil_frequencies(g);
  BOOST_CHECK_EQUAL(boost::num_vertices(g), 6);
  BOOST_CHECK_EQUAL(boost::num_edges(g), 5);
}

BOOST_AUTO_TEST_CASE(ribi_fuse_vertices_with_same_sil_frequencies_two_before_fork)
{
  /*

     1   1   1
   0---1---2---3
           |
           | 1
           +---4

   --+---+-------------------
   # | t | fs (SIL + f)
   --+---+-------------------
   0 | 1 | {{000,2}}
   1 | 2 | {{000,2}}
   2 | 3 | {{000,1},{001,1}}
   3 | 4 | {{000,1}
   4 | 4 | {{001,1}
   --+---+-------------------

  Should become

     2   1
   0---2---3
       |
       | 1
       +---4

   --+---+-------------------
   # | t | fs (SIL + f)
   --+---+-------------------
   0 | 1 | {{000,2}}
   2 | 3 | {{000,1},{001,1}}
   3 | 4 | {{000,1}
   4 | 4 | {{001,1}
   --+---+-------------------

  */
  sil_frequency_phylogeny g;
  const sil sil0{create_sil("000")};
  const sil sil1{create_sil("001")};
  const std::map<sil,int> sfs0 = {{sil0, 2}};
  const std::map<sil,int> sfs1 = {{sil0, 2}};
  const std::map<sil,int> sfs2 = {{sil0, 1}, {sil1, 1}};
  const std::map<sil,int> sfs3 = {{sil0, 1}};
  const std::map<sil,int> sfs4 = {{sil1, 1}};
  const auto sfv0 = sil_frequency_vertex(sfs0, 1);
  const auto sfv1 = sil_frequency_vertex(sfs1, 2);
  const auto sfv2 = sil_frequency_vertex(sfs2, 3);
  const auto sfv3 = sil_frequency_vertex(sfs3, 4);
  const auto sfv4 = sil_frequency_vertex(sfs2, 4);
  const auto vd0 = boost::add_vertex(sfv0,g);
  const auto vd1 = boost::add_vertex(sfv1,g);
  const auto vd2 = boost::add_vertex(sfv2,g);
  const auto vd3 = boost::add_vertex(sfv3,g);
  const auto vd4 = boost::add_vertex(sfv4,g);
  add_sil_frequency_edge(sil_frequency_edge(1), vd0, vd1, g);
  add_sil_frequency_edge(sil_frequency_edge(1), vd1, vd2, g);
  add_sil_frequency_edge(sil_frequency_edge(1), vd2, vd3, g);
  add_sil_frequency_edge(sil_frequency_edge(1), vd2, vd4, g);
  assert(boost::num_vertices(g) == 5);
  assert(boost::num_edges(g) ==  4);
  fuse_vertices_with_same_sil_frequencies(g);
  BOOST_CHECK_EQUAL(boost::num_vertices(g), 4);
  BOOST_CHECK_EQUAL(boost::num_edges(g), 3);
}

BOOST_AUTO_TEST_CASE(ribi_set_all_vertices_styles_incipient)
{
  //Should detect the potential that there can be two species
  //with this genotype pool, would the intermediate be lost
  sil_frequency_phylogeny g;
  const sil sil0{create_sil("000")};
  const sil sil1{create_sil("001")};
  const sil sil2{create_sil("011")};
  const std::map<sil,int> sfs = {{sil0, 1}, {sil1, 1}, {sil2, 1}};
  const auto vd = boost::add_vertex(sil_frequency_vertex(sfs, 1),g);

  assert(boost::num_vertices(g) == 1);
  assert(g[vd].get_style() == sil_frequency_vertex_style::unknown);
  set_all_vertices_styles(g,1);
  BOOST_CHECK_EQUAL(g[vd].get_style(), sil_frequency_vertex_style::incipient);
}

BOOST_AUTO_TEST_CASE(ribi_set_all_vertices_styles_good)
{
  //Should detect the potential that there is only one species
  //with this genotype pool, would any intermediate be lost
  //(due to a genetic distance of 2)
  sil_frequency_phylogeny g;
  const sil sil0{create_sil("000")};
  const sil sil1{create_sil("001")};
  const sil sil2{create_sil("011")};
  const std::map<sil,int> sfs = {{sil0, 1}, {sil1, 1}, {sil2, 1}};
  const auto vd = boost::add_vertex(sil_frequency_vertex(sfs, 1),g);

  assert(boost::num_vertices(g) == 1);
  assert(g[vd].get_style() == sil_frequency_vertex_style::unknown);
  set_all_vertices_styles(g,2);
  BOOST_CHECK_EQUAL(g[vd].get_style(), sil_frequency_vertex_style::good);
}


// From a population, create a single node phylogeny:
//
// +----+
// | 00 | <- SIL
// | 02 | <- frequency
// +----+
BOOST_AUTO_TEST_CASE(results_create_single_node_phylogeny)
{
  #ifdef FIX_ISSUE_41
  const int max_genetic_distance{1};
  results r(max_genetic_distance);
  const pin my_pin("");
  const sil my_sil(2, 0b00);
  std::vector<individual> p{
    individual(my_pin, my_sil)
  };

  r.add_measurement(0, p);

  const auto g = r.get_sil_frequency_phylogeny();
  BOOST_REQUIRE_EQUAL(boost::num_vertices(g), 1);
  BOOST_CHECK_EQUAL(g[*vertices(g).first].get_sil_frequencies().size(), 1);
  #endif // FIX_ISSUE_41
}

/*

The initial state in data collection looks as such:

```
+--+--+--+--+--+--+--+--+--+--+
|G |t1|t2|t3|t4|t5|t6|t7|t8|t9|
+--+--+--+--+--+--+--+--+--+--+
|00| 9| 9| 9| 9| 9| 9| 9| 9| 9|
+--+--+--+--+--+--+--+--+--+--+
```

During measurements: create a graph from this, connecting the species:

```
     9--9--9--9--9--9--9--9--9
```

Next step: summarize genotypes:

```
     9--9--9--9--9--9--9--9--9
```

Next step: summarize edges (the number above the edge denotes its length):

```
   8
 *---*
```

*/
BOOST_AUTO_TEST_CASE(results_summarize_to_short)
{
  #ifdef FIX_ISSUE_41

  //+--+--+--+--+--+--+--+--+--+--+
  //|G |t1|t2|t3|t4|t5|t6|t7|t8|t9|
  //+--+--+--+--+--+--+--+--+--+--+
  //|00| 9| 9| 9| 9| 9| 9| 9| 9| 9|
  //+--+--+--+--+--+--+--+--+--+--+
  const individual i00(dna(""), sil(2,0b00));

  std::vector<population> populations(9, population_factory().create(9, i00));
  assert(populations.size() == 9);
  const int n_populations{static_cast<int>(populations.size())};
  const int max_genetic_distance{1};
  results r(max_genetic_distance);
  for (int t=0; t!=n_populations; ++t)
  {
    assert(t >= 0 && t < static_cast<int>(populations.size()));
    r.add_measurement(t, populations[t]);
  }
  r.summarize_sil_frequency_phylogeny();
  const auto g = r.get_summarized_sil_frequency_phylogeny();
  /*
       8
     *---*
  */
  BOOST_CHECK_EQUAL(boost::num_vertices(g), 2);
  BOOST_CHECK_EQUAL(boost::num_edges(g), 1);
  #endif // FIX_ISSUE_41
}


/*

The initial state in data collection looks as such:

```
+--+--+--+--+--+--+--+--+--+--+
|G |t1|t2|t3|t4|t5|t6|t7|t8|t9|
+--+--+--+--+--+--+--+--+--+--+
|00| 9| 8| 6| 5| 4| 4| 4| 4| 4|
|01|  | 1| 2| 2| 2| 1|  |  |  |
|11|  |  | 1| 2| 3| 4| 5| 5| 5|
|10|  |  |  |  |  |  |  |  |  |
+--+--+--+--+--+--+--+--+--+--+
```

During measurements: create a graph from this, connecting the species:

```
     9--8--6--5--4--4--4--4--4
      \ |  |  |  |  | /
       -1--2--2--2--1
         \ |  |  |  | \
          -1--2--3--4--5--5--5

```

Next step: summarize genotypes:

 * One species possible: keep solid line
 * More species possible: use dotted line

```
           +--- speciation initiation
           |        +--- speciation completion
           |        |
           v        v
     9--9--9..9..9..9--4--4--4
                     \-5--5--5
```

Next step: summarize edges (the number above the edge denotes its length):

```
   1   1   3   1   2
 *---*->.*...*.<-*---*
             .
             . 1   2
             +.<-*---*
```

*/
BOOST_AUTO_TEST_CASE(results_example_complete_speciation)
{
  #ifdef FIX_ISSUE_41

  //+--+--+--+--+--+--+--+--+--+--+
  //|G |t1|t2|t3|t4|t5|t6|t7|t8|t9|
  //+--+--+--+--+--+--+--+--+--+--+
  //|00| 9| 8| 6| 5| 4| 4| 4| 4| 4|
  //|01|  | 1| 2| 2| 2| 1|  |  |  |
  //|11|  |  | 1| 2| 3| 4| 5| 5| 5|
  //|10|  |  |  |  |  |  |  |  |  |
  //+--+--+--+--+--+--+--+--+--+--+
  //|  | G| G| I| I| I| I|GG|GG|GG|
  //+--+--+--+--+--+--+--+--+--+--+

  const individual i00(dna(""), sil(2,0b00));
  const individual i01(dna(""), sil(2,0b01));
  const individual i11(dna(""), sil(2,0b11));

  std::vector<population> populations{
    population_factory().create(9, i00, 0, i01, 0, i11),
    population_factory().create(8, i00, 1, i01, 0, i11),
    population_factory().create(6, i00, 2, i01, 1, i11),
    population_factory().create(5, i00, 2, i01, 2, i11),
    population_factory().create(4, i00, 2, i01, 3, i11),
    population_factory().create(4, i00, 1, i01, 4, i11),
    population_factory().create(4, i00, 0, i01, 5, i11),
    population_factory().create(4, i00, 0, i01, 5, i11),
    population_factory().create(4, i00, 0, i01, 5, i11)
  };
  assert(populations.size() == 9);
  const int n_populations{static_cast<int>(populations.size())};
  const int max_genetic_distance{1};
  results r(max_genetic_distance);
  for (int t=0; t!=n_populations; ++t)
  {
    assert(t >= 0 && t < static_cast<int>(populations.size()));
    r.add_measurement(t, populations[t]);
  }
  r.summarize_sil_frequency_phylogeny();
  const auto g = r.get_summarized_sil_frequency_phylogeny();
  /*

     2   4   2
   G---I...G---G
       |
       | 4   2
       +...G---G

  */
  BOOST_CHECK_EQUAL(boost::num_vertices(g), 7);
  BOOST_CHECK_EQUAL(boost::num_edges(g), 6);

  #endif // FIX_ISSUE_41
}

/*


This also allows for unsuccessful speciation-initiations:

```
+--+--+--+--+--+--+--+--+--+--+
|G |t1|t2|t3|t4|t5|t6|t7|t8|t9|
+--+--+--+--+--+--+--+--+--+--+
|00| 9| 8| 6| 5| 4| 4| 5| 5| 4|
|01|  | 1| 2| 2| 2| 1| 2| 4| 5|
|11|  |  | 1| 2| 3| 4| 2|  |  |
|10|  |  |  |  |  |  |  |  |  |
+--+--+--+--+--+--+--+--+--+--+
```

Next step: create a graph from this:

```
     9--8--6--5--4--4--5--5--4
        1--2--2--2--1--2--4--5
           1--2--3--4--2

```

Next step: connect the species:

```
     9--8--6--5--4--4--5--5--4
        |  |  |  |  |  |  |  |
        1--2--2--2--1--2--4--5
           |  |  |  |  |
           1--2--3--4--2

```

Next step: summarize species:

 * One species possible: keep solid line
 * More species possible: use dotted line

```
           +--- speciation initiation
           |           +--- end of speciation-initiation
           |           |
           v           v
     9--9--9..9..9..9..9--9--9
```

Next step: summarize edges (the number above the edge denotes its length):

```
   1   1   4   1   1
 *---*->-*...*.>-*---*
```

*/
BOOST_AUTO_TEST_CASE(results_example_unsuccessful_speciation)
{
  #ifdef FIX_ISSUE_41
  // +--+--+--+--+--+--+--+--+--+--+
  // |G |t1|t2|t3|t4|t5|t6|t7|t8|t9|
  // +--+--+--+--+--+--+--+--+--+--+
  // |00| 9| 8| 6| 5| 4| 4| 5|  |  |
  // |01|  | 1| 2| 2| 2| 1| 2| 4| 5|
  // |11|  |  | 1| 2| 3| 4| 2| 5| 4|
  // |10|  |  |  |  |  |  |  |  |  |
  // +--+--+--+--+--+--+--+--+--+--+
  // |  | G| G| I| I| I| I| I| G| G|
  // +--+--+--+--+--+--+--+--+--+--+

  const individual i00(dna(""), sil(2,0b00));
  const individual i01(dna(""), sil(2,0b01));
  const individual i11(dna(""), sil(2,0b11));

  std::vector<population> populations{
    population_factory().create(
      9, i00,
      0, i01,
      0, i11
    ),
    population_factory().create(
      8, i00,
      1, i01,
      0, i11
    ),
    population_factory().create(
      6, i00,
      2, i01,
      1, i11
    ),
    population_factory().create(
      5, i00,
      2, i01,
      2, i11
    ),
    population_factory().create(
      4, i00,
      2, i01,
      3, i11
    ),
    population_factory().create(
      4, i00,
      1, i01,
      4, i11
    ),
    population_factory().create(
      5, i00,
      2, i01,
      2, i11
    ),
    population_factory().create(
      0, i00,
      4, i01,
      5, i11
    ),
    population_factory().create(
      0, i00,
      5, i01,
      4, i11
    )
  };
  assert(populations.size() == 9);
  const int n_populations{static_cast<int>(populations.size())};
  const int max_genetic_distance{1};
  results r(max_genetic_distance);
  for (int t=0; t!=n_populations; ++t)
  {
    assert(t >= 0 && t < static_cast<int>(populations.size()));
    r.add_measurement(t, populations[t]);
  }
  r.summarize_sil_frequency_phylogeny();
  const auto g = r.get_summarized_sil_frequency_phylogeny();
 /*

   2   5   1
 G---I->-G---G

  */
  BOOST_CHECK_EQUAL(boost::num_vertices(g), 4);
  BOOST_CHECK_EQUAL(boost::num_edges(g), 3);
  #endif // FIX_ISSUE_41
}



/*

There is a problem case, I do not think is a problem:


```
+---+--+--+--+--+--+--+--+--+--+
| G |t1|t2|t3|t4|t5|t6|t7|t8|t9|
+---+--+--+--+--+--+--+--+--+--+
|000|  |  |  | 2| 2| 2| 2|  |  |
|001|  | 3| 3| 2| 2| 2| 2| 3|  |
|011| 9| 3| 3|  |  |  | 1| 3| 9|
|010|  | 3| 3| 3| 3| 3| 2| 3|  |
|110|  |  |  | 2| 2| 2| 2|  |  |
+---+--+--+--+--+--+--+--+--+--+
```

Next step: create a graph from this:

```
            2--2--2--2
      3--3--2--2--2--2--3
   9--3--3           1--3--9
      3--3--3--3--3--2--3
            2--2--2--2

```

Next step: connect the species:

```
            2--2--2--2
            |  |  |  |
      3--3--2--2--2--2--3
      |  |           |  |
   9--3--3           1--3--9
      |  |           |  |
      3--3--3--3--3--2--3
            |  |  |  |
            2--2--2--2

```

Next step: summarize species:

 * One species possible: keep solid line
 * More species possible: use dotted line

```
      +--- speciation initiation
      |  +--- end of speciation-initiation
      |  |
      v  v
   9--9..9--4--4--4--9--9--9
          \         /
           \       /
            5--5--5

```

Next step: smash loops, use an equals (`=`) line here:

```
      +--- speciation initiation
      |              +--- end of speciation-initiation
      |              |
      v              v
   9--9..9==9==9==9==9--9--9

```

An equals (`=`) line denotes: there were two species, that could reform the
missing link again. These occurrances should be rare for more loci! I
intend to treat these lineages as incipient species, as they could
not keep a good species established.

Next step: summarize edges (the number above the edge denotes its length):

```
    1   1   4   2
  9---9...9===9---9
```
*/
BOOST_AUTO_TEST_CASE(results_example_problem_case)
{
  #ifdef FIX_ISSUE_41
  // +---+--+--+--+--+--+--+--+--+--+
  // | G |t1|t2|t3|t4|t5|t6|t7|t8|t9|
  // +---+--+--+--+--+--+--+--+--+--+
  // |000|  |  |  | 2| 2| 2| 2|  |  |
  // |001|  | 3| 3| 2| 2| 2| 2| 3|  |
  // |011| 9| 3| 3|  |  |  | 1| 3| 9|
  // |111|  | 3| 3| 3| 3| 3| 2| 3|  |
  // |110|  |  |  | 2| 2| 2| 2|  |  |
  // +---+--+--+--+--+--+--+--+--+--+
  const individual i000(dna(""), sil(3,0b000));
  const individual i001(dna(""), sil(3,0b001));
  const individual i011(dna(""), sil(3,0b011));
  const individual i111(dna(""), sil(3,0b111));
  const individual i110(dna(""), sil(3,0b110));

  std::vector<population> populations{
    population_factory().create(0, i000, 0, i001, 9, i011, 0, i111, 0, i110),
    population_factory().create(0, i000, 3, i001, 3, i011, 3, i111, 0, i110),
    population_factory().create(0, i000, 3, i001, 3, i011, 3, i111, 0, i110),
    population_factory().create(2, i000, 2, i001, 0, i011, 3, i111, 2, i110),
    population_factory().create(2, i000, 2, i001, 0, i011, 3, i111, 2, i110),
    population_factory().create(2, i000, 2, i001, 0, i011, 3, i111, 2, i110),
    population_factory().create(2, i000, 2, i001, 1, i011, 2, i111, 2, i110),
    population_factory().create(0, i000, 3, i001, 3, i011, 3, i111, 0, i110),
    population_factory().create(0, i000, 0, i001, 9, i011, 0, i111, 0, i110)
  };
  assert(populations.size() == 9);
  const int n_populations{static_cast<int>(populations.size())};
  const int max_genetic_distance{1};
  results r(max_genetic_distance);
  for (int t=0; t!=n_populations; ++t)
  {
    assert(t >= 0 && t < static_cast<int>(populations.size()));
    r.add_measurement(t, populations[t]);
  }
  r.summarize_sil_frequency_phylogeny();
  const auto g = r.get_summarized_sil_frequency_phylogeny();
  /*
    1   1   4   2
  9---9...9===9---9

  */
  BOOST_CHECK_EQUAL(boost::num_vertices(g), 5);
  BOOST_CHECK_EQUAL(boost::num_edges(g), 4);
  #endif // FIX_ISSUE_41
}

/*

Here is an even harder case:


```
+---+--+--+--+--+--+--+--+--+--+
| G |t1|t2|t3|t4|t5|t6|t7|t8|t9|
+---+--+--+--+--+--+--+--+--+--+
|000|  |  |  |  |  |  |  |  |  |
|001|  | 3| 3| 3| 3| 3| 2|  |  |
|011| 9| 3|  |  |  |  | 1| 3|  |
|010|  | 3| 3| 2| 3| 3| 3| 3| 9|
|110|  |  | 3| 2|  |  | 2| 3|  |
|111|  |  |  | 2| 1| 1| 1|  |  |
|101|  |  |  |  | 1| 1|  |  |  |
|100|  |  |  |  | 1| 1|  |  |  |
+---+--+--+--+--+--+--+--+--+--+
```

Next step: create a graph from this:

```
 001     3--3--3--3--3--2
 011  9--3              1--3
 010     3--3--2--3--3--3--3--9
 110        3--2        2--3
 111           2--1--1--1
 101              1--1
 100              1--1

```

Next step: connect the species:

```
 001     3--3--3--3--3--2
         |              |
 011  9--3              1--3
         |              |  |
 010     3--3--2--3--3--3--3--9
            |  |        |  |
 110        3--2        2--3
               |        |
 111           2--1--1--1
                  |  |
 101              1--1
                  |  |
 100              1--1


```

Next step: summarize species:

 * One species possible: keep solid line
 * More species possible: use dotted line

```
      +--- speciation initiation
      |  +--- end of speciation-initiation
      |  |
      v  v
   9--9..3--3--3--3
       \           \
        \           \
         6..6--3--3--9--9--9
             \      /
              \    /
               3..3
```

I will have to think how to solve this,
I suggest to just get rid of the complexity (as it is rare)
and merge to

  9--9..9==9==9==9==9--9--9

*/

BOOST_AUTO_TEST_CASE(ribi_results_abuse)
{
  BOOST_CHECK_NO_THROW(
    results(1) //max_genetic_distance
  );

  //Cannot have a negative genetic distance
  BOOST_CHECK_THROW(
    results(-1), //max_genetic_distance
    std::invalid_argument
  );

  //Cannot add a measurement at a negative t
  {
    results r(1);
    BOOST_CHECK_THROW(
      r.add_measurement(-1, population()),
      std::invalid_argument
    );
  }
}

BOOST_AUTO_TEST_CASE(ribi_remove_vertex_with_id_use)
{
  //Cannot remove a non-existing vertex
  sil_frequency_phylogeny g;
  const std::map<sil,int> sfs = {{create_sil("000"), 1}};
  const auto vd = boost::add_vertex(sil_frequency_vertex(sfs, 1),g);
  const auto id = g[vd].get_id();

  BOOST_CHECK_EQUAL(boost::num_vertices(g), 1);
  BOOST_CHECK_NO_THROW(remove_vertex_with_id(id, g));
  BOOST_CHECK_EQUAL(boost::num_vertices(g), 0);
}

BOOST_AUTO_TEST_CASE(ribi_remove_vertex_with_id_abuse)
{
  //Cannot remove a non-existing vertex
  sil_frequency_phylogeny g;
  BOOST_CHECK_THROW(
    remove_vertex_with_id(123, g),
    std::invalid_argument
  );
}


BOOST_AUTO_TEST_CASE(ribi_find_splits_and_mergers_right_size_1)
{
  /*
             2
            / \
   Past 0--1   4--5 Present
            \ /
             3
  */
  sil_frequency_phylogeny g = get_test_sil_frequency_phylogeny_1();
  const auto v = find_splits_and_mergers(g);
  BOOST_CHECK(v.size() == 1);
}

BOOST_AUTO_TEST_CASE(ribi_find_splits_and_mergers_right_size_2)
{
  /*
             2--4
            /    \
   Past 0--1      6--7 Present
            \    /
             3--5
  */
  sil_frequency_phylogeny g = get_test_sil_frequency_phylogeny_2();
  const auto v = find_splits_and_mergers(g);
  BOOST_CHECK(v.size() == 1);
}

BOOST_AUTO_TEST_CASE(ribi_find_splits_and_mergers_right_spots_1)
{
  /*
           +------- merger
           |   +--- split
           |   |
           v   v
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

  */
  sil_frequency_phylogeny g = get_test_sil_frequency_phylogeny_1();
  const auto v = find_splits_and_mergers(g);
  assert(v.size() == 1);
  const auto split_and_merger = v[0];
  const auto vd_split = split_and_merger.first;
  const auto vd_merger = split_and_merger.second;
  const std::map<sil,int>& sfs_split  = g[vd_split ].get_sil_frequencies();
  const std::map<sil,int>& sfs_merger = g[vd_merger].get_sil_frequencies();
  BOOST_CHECK_EQUAL(sfs_split.size(), 1);
  BOOST_CHECK_EQUAL(sfs_merger.size(), 1);
  BOOST_CHECK_EQUAL(sfs_split.count( create_sil("100")), 1);
  BOOST_CHECK_EQUAL(sfs_split.count( create_sil("001")), 0);
  BOOST_CHECK_EQUAL(sfs_merger.count(create_sil("100")), 0);
  BOOST_CHECK_EQUAL(sfs_merger.count(create_sil("001")), 1);
}

BOOST_AUTO_TEST_CASE(ribi_find_splits_and_mergers_right_spots_2)
{
  /*
           +---------- merger
           |      +--- split
           |      |
           v      v
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
  */
  sil_frequency_phylogeny g = get_test_sil_frequency_phylogeny_2();
  const auto v = find_splits_and_mergers(g);
  assert(v.size() == 1);
  const auto split_and_merger = v[0];
  const auto vd_split = split_and_merger.first;
  const auto vd_merger = split_and_merger.second;
  const std::map<sil,int>& sfs_split  = g[vd_split ].get_sil_frequencies();
  const std::map<sil,int>& sfs_merger = g[vd_merger].get_sil_frequencies();
  BOOST_CHECK_EQUAL(sfs_split.size(), 1);
  BOOST_CHECK_EQUAL(sfs_merger.size(), 1);
  BOOST_CHECK_EQUAL(sfs_split.count( create_sil("100")), 1);
  BOOST_CHECK_EQUAL(sfs_split.count( create_sil("001")), 0);
  BOOST_CHECK_EQUAL(sfs_merger.count(create_sil("100")), 0);
  BOOST_CHECK_EQUAL(sfs_merger.count(create_sil("001")), 1);
}

BOOST_AUTO_TEST_CASE(ribi_get_older_1)
{
  /*
           +------- merger
           |   +--- split
           |   |
           v   v
             2
            / \
   Past 0--1   4--5 Present
            \ /
             3

  */
  sil_frequency_phylogeny g = get_test_sil_frequency_phylogeny_1();
  const auto v = find_splits_and_mergers(g);
  assert(v.size() == 1);
  const auto split_and_merger = v[0];
  const auto vd_split = split_and_merger.first;
  const auto vd_merger = split_and_merger.second;
  BOOST_CHECK_EQUAL(get_older(vd_split, g).size(), 2);
  BOOST_CHECK_EQUAL(get_older(vd_merger, g).size(), 1);
  BOOST_CHECK_EQUAL(get_younger(vd_split, g).size(), 1);
  BOOST_CHECK_EQUAL(get_younger(vd_merger, g).size(), 2);

  //Now both vds at the same time
  const sil_frequency_vertex_descriptors vds = { vd_split, vd_merger};
  BOOST_CHECK_EQUAL(get_younger(vds, g).size(), 3);
}

BOOST_AUTO_TEST_CASE(ribi_get_older_2)
{
  /*
           +---------- merger
           |      +--- split
           |      |
           v      v
             2--4
            /    \
   Past 0--1      6--7 Present
            \    /
             3--5
  */
  sil_frequency_phylogeny g = get_test_sil_frequency_phylogeny_2();
  const auto v = find_splits_and_mergers(g);
  assert(v.size() == 1);
  const auto split_and_merger = v[0];
  const auto vd_split = split_and_merger.first;
  const auto vd_merger = split_and_merger.second;
  BOOST_CHECK_EQUAL(get_older(vd_split, g).size(), 2);
  BOOST_CHECK_EQUAL(get_older(vd_merger, g).size(), 1);
  BOOST_CHECK_EQUAL(get_younger(vd_split, g).size(), 1);
  BOOST_CHECK_EQUAL(get_younger(vd_merger, g).size(), 2);
}

BOOST_AUTO_TEST_CASE(ribi_zip_1)
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

   should become:

   Past 0--1--6--4--5 Present

   --+---+-------------------
   # | t | fs (SIL + f)
   --+---+-------------------
   0 | 1 | {{000,2}}
   1 | 2 | {{001,2}}
   4 | 4 | {{100,2}}
   5 | 5 | {{101,2}}
   6 | 3 | {{010,1},{011,1}}
   --+---+-------------------

  */

  sil_frequency_phylogeny g = get_test_sil_frequency_phylogeny_1();
  BOOST_CHECK(is_isomorphic(g, get_test_sil_frequency_phylogeny_1()));
  zip(g);
  BOOST_CHECK_EQUAL(boost::num_vertices(g), 5);
  BOOST_CHECK_EQUAL(boost::num_edges(g), 4);
  BOOST_CHECK(!is_isomorphic(g, get_test_sil_frequency_phylogeny_1()));
}

BOOST_AUTO_TEST_CASE(ribi_zip_2_once)
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

   should become:

          1   1   1   1   1
   Past 0---1---8---9---6---7 Present

   --+---+-------------------
   # | t | fs (SIL + f)
   --+---+-------------------
   0 | 1 | {{000,2}}
   1 | 2 | {{001,2}}
   6 | 5 | {{100,2}}
   7 | 6 | {{101,2}}
   8 | 3 | {{010,1},{011,1}}
   9 | 4 | {{010,1},{011,1}}
   --+---+-------------------

  Note that the phylogeny can be simplied with
  the function 'fuse_vertices_with_same_sil_frequencies'
  to this:

          1   1   2   1
   Past 0---1---8---6---7 Present

  */
  //Get the splits and mergers and zip from those

  sil_frequency_phylogeny g = get_test_sil_frequency_phylogeny_2();
  const auto v = find_splits_and_mergers(g);
  assert(v.size() == 1);
  const auto split_and_merger = v[0];
  zip(split_and_merger, g);
  //zip does not remove the unconnected_empty_vertices
  remove_unconnected_empty_vertices(g);

  BOOST_CHECK_EQUAL(boost::num_vertices(g), 6);
  BOOST_CHECK_EQUAL(boost::num_edges(g), 5);
  BOOST_CHECK(!is_isomorphic(g, get_test_sil_frequency_phylogeny_2()));
}

BOOST_AUTO_TEST_CASE(ribi_zip_2)
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

   should become:

          1   1   1   1   1
   Past 0---1---8---9---6---7 Present

   --+---+-------------------
   # | t | fs (SIL + f)
   --+---+-------------------
   0 | 1 | {{000,2}}
   1 | 2 | {{001,2}}
   6 | 5 | {{100,2}}
   7 | 6 | {{101,2}}
   8 | 3 | {{010,1},{011,1}}
   9 | 4 | {{010,1},{011,1}}
   --+---+-------------------

  Note that the phylogeny can be simplied with
  the function 'fuse_vertices_with_same_sil_frequencies'
  to this:

          1   1   2   1
   Past 0---1---8---6---7 Present

  */

  sil_frequency_phylogeny g = get_test_sil_frequency_phylogeny_2();
  BOOST_CHECK(is_isomorphic(g, get_test_sil_frequency_phylogeny_2()));
  zip(g);
  BOOST_CHECK_EQUAL(boost::num_vertices(g), 6);
  BOOST_CHECK_EQUAL(boost::num_edges(g), 5);
  BOOST_CHECK(!is_isomorphic(g, get_test_sil_frequency_phylogeny_2()));
}

#pragma GCC diagnostic pop
