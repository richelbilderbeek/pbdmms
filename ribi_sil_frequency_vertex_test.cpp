#include "ribi_sil_frequency_vertex.h"

#include <sstream>
#include <boost/graph/adjacency_list.hpp>
#include "ribi_population_factory.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace ribi;

BOOST_AUTO_TEST_CASE(test_results_vertex_default_construction)
{
  sil_frequency_vertex v;
  BOOST_CHECK(v.get_sil_frequencies().empty());
  BOOST_CHECK_EQUAL(v.get_time(), 0);
}

BOOST_AUTO_TEST_CASE(test_merge_sil_frequencies)
{
  // Normal collecting
  // lhs = {{00, 1}}, rhs = {{11, 2}} -> {{00, 1}, {11, 2}}
  {
    std::map<sil,int> fs_a;
    fs_a.insert(std::make_pair(sil(2,0b00),1));
    std::map<sil,int> fs_b;
    fs_b.insert(std::make_pair(sil(2,0b11),2));
    std::map<sil,int> fs_expected;
    fs_expected.insert(std::make_pair(sil(2,0b00),1));
    fs_expected.insert(std::make_pair(sil(2,0b11),2));
    BOOST_CHECK(merge_sil_frequencies(fs_a, fs_b) == fs_expected);
  }
  // Simple summarization
  // lhs = {{00, 1}}, rhs = {{00, 2}} -> {{00, 3}}
  {
    std::map<sil,int> fs_a;
    fs_a.insert(std::make_pair(sil(2,0b00),1));
    std::map<sil,int> fs_b;
    fs_b.insert(std::make_pair(sil(2,0b00),2));
    std::map<sil,int> fs_expected;
    fs_expected.insert(std::make_pair(sil(2,0b00),3));
    BOOST_CHECK(merge_sil_frequencies(fs_a, fs_b) == fs_expected);
  }
  // Another simple summarization
  // lhs = {{00, 1}}, rhs = {{00, 3}} -> {{00, 4}}
  {
    std::map<sil,int> fs_a;
    fs_a.insert(std::make_pair(sil(2,0b00),1));
    std::map<sil,int> fs_b;
    fs_b.insert(std::make_pair(sil(2,0b00),3));
    std::map<sil,int> fs_expected;
    fs_expected.insert(std::make_pair(sil(2,0b00),4));
    BOOST_CHECK(merge_sil_frequencies(fs_a, fs_b) == fs_expected);
  }
  // Harder summarization
  /// lhs = {{00, 1}}, rhs = {{00, 2}, {11, 1}} -> {{00, 3}, {11, 1}}
  {
    std::map<sil,int> fs_a;
    fs_a.insert(std::make_pair(sil(2,0b00),1));
    std::map<sil,int> fs_b;
    fs_b.insert(std::make_pair(sil(2,0b00),2));
    fs_b.insert(std::make_pair(sil(2,0b11),1));
    std::map<sil,int> fs_expected;
    fs_expected.insert(std::make_pair(sil(2,0b00),3));
    fs_expected.insert(std::make_pair(sil(2,0b11),1));
    BOOST_CHECK(merge_sil_frequencies(fs_a, fs_b) == fs_expected);
  }
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

BOOST_AUTO_TEST_CASE(test_move_sil_frequencies_abuse)
{
  //Cannot move SIL frequencies from different timepoints
  std::map<sil,int> fs_a;
  std::map<sil,int> fs_b;
  fs_a.insert(std::make_pair(sil(2,0b00),10));
  fs_b.insert(std::make_pair(sil(2,0b11),20));
  const int t1{42};
  const int t2{123};
  sil_frequency_vertex a(fs_a, t1);
  sil_frequency_vertex b(fs_b, t2);
  BOOST_CHECK_THROW(
    move_sil_frequencies(a,b),
    std::invalid_argument
  );
}

BOOST_AUTO_TEST_CASE(test_move_sil_frequencies_is_duplicate_detected)
{
  //If a {{SIL,frequency}} map is moved to another {{SIL,frequency}} map,
  //those maps are fused, leaving the 'from' map empty and the 'to'
  //map having the fused map
  std::map<sil,int> fs_a;
  std::map<sil,int> fs_b;
  fs_a.insert(std::make_pair(sil(2,0b00),10));
  fs_b.insert(std::make_pair(sil(2,0b00),20));
  sil_frequency_vertex a(fs_a, 42);
  sil_frequency_vertex b(fs_b, 42);
  BOOST_CHECK_EQUAL(a.get_sil_frequencies().size(), 1);
  BOOST_CHECK_EQUAL(b.get_sil_frequencies().size(), 1);
  BOOST_CHECK_NO_THROW(move_sil_frequencies(a,b));
  BOOST_CHECK_EQUAL(a.get_sil_frequencies().size(), 0);
  BOOST_CHECK_EQUAL(b.get_sil_frequencies().size(), 1);
}

BOOST_AUTO_TEST_CASE(test_sil_frequency_vertex_streaming)
{
  std::map<sil,int> fs;
  fs.insert(std::make_pair(sil(2,0b00),10));
  const sil_frequency_vertex a(fs, 42);
  std::stringstream s;
  s << a;
  BOOST_CHECK(!s.str().empty());
}

BOOST_AUTO_TEST_CASE(test_ribi_sil_frequency_vertex_abuse)
{
  //SIL frequencies cannot be negative
  {
    std::map<sil,int> fs;
    fs.insert(std::make_pair(sil(2,0b00),-1)); //-1 is a negative (thus incorrect) frequency
    BOOST_CHECK_THROW(sil_frequency_vertex(fs, 42), std::invalid_argument);
  }
  //Time cannot be negative
  {
    std::map<sil,int> fs;
    fs.insert(std::make_pair(sil(2,0b00),1));
    BOOST_CHECK_THROW(sil_frequency_vertex(fs, -42), std::invalid_argument);
  }
}

BOOST_AUTO_TEST_CASE(test_ribi_sil_frequencies_to_str)
{
  std::map<sil,int> sfs; //SIL frequencies
  sfs.insert(std::make_pair(sil(4,0b0101),42));
  const std::string expected{"0101: 42"}
  BOOST_CHECK_EQUAL(sil_frequencies_to_str(sfs), expected);
}



#pragma GCC diagnostic pop

