#include "distancer_sil_frequency_vertex.h"
#include <boost/test/unit_test.hpp>
#include <boost/graph/adjacency_list.hpp>
#include "distancer_population_factory.h"

BOOST_AUTO_TEST_CASE(test_results_vertex_default_construction)
{
  sil_frequency_vertex v;
  BOOST_CHECK(v.get_sil_frequencies().empty());
  BOOST_CHECK_EQUAL(v.get_time(), 0);
}

BOOST_AUTO_TEST_CASE(test_move_sil_frequencies)
{
  std::map<sil,int> fs_a;
  std::map<sil,int> fs_b;
  fs_a.insert(std::make_pair(sil(2,0b00),10));
  fs_b.insert(std::make_pair(sil(2,0b11),20));
  sil_frequency_vertex a(fs_a, 42);
  sil_frequency_vertex b(fs_b, 42);
  BOOST_CHECK_EQUAL(a.get_sil_frequencies().size(), 1);
  BOOST_CHECK_EQUAL(b.get_sil_frequencies().size(), 1);
  move_sil_frequencies(a,b);
  BOOST_CHECK_EQUAL(a.get_sil_frequencies().size(), 0);
  BOOST_CHECK_EQUAL(b.get_sil_frequencies().size(), 2);
}

BOOST_AUTO_TEST_CASE(test_move_sil_frequencies_is_duplicate_detected)
{
  //If there are two SILs (which should not happen),
  //then move_sil_frequencies should throw, without modifying the original vertices
  std::map<sil,int> fs_a;
  std::map<sil,int> fs_b;
  fs_a.insert(std::make_pair(sil(2,0b00),10));
  fs_b.insert(std::make_pair(sil(2,0b00),20)); //SAME SIL!
  sil_frequency_vertex a(fs_a, 42);
  sil_frequency_vertex b(fs_b, 42);
  BOOST_CHECK_EQUAL(a.get_sil_frequencies().size(), 1);
  BOOST_CHECK_EQUAL(b.get_sil_frequencies().size(), 1);
  BOOST_CHECK_THROW(move_sil_frequencies(a,b), std::invalid_argument);
  BOOST_CHECK_EQUAL(a.get_sil_frequencies().size(), 1); //Untouched
  BOOST_CHECK_EQUAL(b.get_sil_frequencies().size(), 1); //Untouched
}

