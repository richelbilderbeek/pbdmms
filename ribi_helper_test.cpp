#include "ribi_helper.h"

#include <iostream>
#include <fstream>
#include <boost/dynamic_bitset.hpp>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace ribi;

BOOST_AUTO_TEST_CASE(test_boost_dynamic_bitset)
{
  {
    const size_t n_loci{4};
    boost::dynamic_bitset<> p(n_loci, 0b1111);
    p.flip(2); //From right
    boost::dynamic_bitset<> p_should_be(n_loci, 0b1011);
    BOOST_CHECK(p == p_should_be);
  }
  {
    const size_t n_loci{4};
    boost::dynamic_bitset<> p(n_loci, 0b0000);
    p.flip(2); //From right
    boost::dynamic_bitset<> p_should_be(n_loci, 0b0100);
    BOOST_CHECK(p == p_should_be);
  }
  {
    const size_t n_loci{4};
    boost::dynamic_bitset<> p(n_loci, 15);
    boost::dynamic_bitset<> p_should_be(n_loci, 0b1111);
    BOOST_CHECK(p == p_should_be);
  }
  {
    const size_t n_loci{4};
    boost::dynamic_bitset<> p(n_loci, 7);
    boost::dynamic_bitset<> p_should_be(n_loci, 0b0111);
    BOOST_CHECK(p == p_should_be);
  }
  //Inheritance
  {
    const size_t n_loci{3};
    const boost::dynamic_bitset<> p(n_loci, 0b001);
    const boost::dynamic_bitset<> q(n_loci, 0b100);
    const boost::dynamic_bitset<> r = p ^ q;
    const boost::dynamic_bitset<> inherit_from_p(n_loci, 0b001);
    const boost::dynamic_bitset<> kid = (inherit_from_p & p) | (~inherit_from_p & q);
    const boost::dynamic_bitset<> kid_should_be(n_loci, 0b101);
    BOOST_CHECK(kid == kid_should_be);
  }
  {
    const size_t n_loci{4};
    const boost::dynamic_bitset<> p(n_loci, 0b0000);
    const boost::dynamic_bitset<> q(n_loci, 0b1111);
    const boost::dynamic_bitset<> inherit_from_p(n_loci, 0b0101);
    const boost::dynamic_bitset<> kid = (inherit_from_p & p) | (~inherit_from_p & q);
    const boost::dynamic_bitset<> kid_should_be(n_loci, 0b1010);
    BOOST_CHECK(kid == kid_should_be);
  }
  {
    const size_t n_loci{4};
    const boost::dynamic_bitset<> p(n_loci, 0b1111);
    const boost::dynamic_bitset<> q(n_loci, 0b0000);
    const boost::dynamic_bitset<> inherit_from_p(n_loci, 0b0101);
    const boost::dynamic_bitset<> kid = (inherit_from_p & p) | (~inherit_from_p & q);
    const boost::dynamic_bitset<> kid_should_be(n_loci, 0b0101);
    BOOST_CHECK(kid == kid_should_be);
  }
}




BOOST_AUTO_TEST_CASE(test_count_connected_components)
{
  {
    boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> g;
    boost::add_vertex(g);
    BOOST_CHECK(::count_connected_components(g) == 1);
  }
  {
    boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> g;
    boost::add_vertex(g);
    boost::add_vertex(g);
    BOOST_CHECK(::count_connected_components(g) == 2);
  }
  {
    boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> g;
    const auto vd_a = boost::add_vertex(g);
    const auto vd_b = boost::add_vertex(g);
    boost::add_edge(vd_a, vd_b, g);
    BOOST_CHECK(::count_connected_components(g) == 1);
  }
}

BOOST_AUTO_TEST_CASE(test_count_different_bits_use)
{
  const size_t n_loci{3};
  const boost::dynamic_bitset<> a(n_loci, 0b000); //0b denotes binary
  const boost::dynamic_bitset<> b(n_loci, 0b001);
  const boost::dynamic_bitset<> c(n_loci, 0b011);
  BOOST_CHECK(::count_different_bits(a,a) == 0);
  BOOST_CHECK(::count_different_bits(b,b) == 0);
  BOOST_CHECK(::count_different_bits(a,b) == 1);
  BOOST_CHECK(::count_different_bits(b,a) == 1);
  BOOST_CHECK(::count_different_bits(c,a) == 2);
  BOOST_CHECK(::count_different_bits(a,c) == 2);
}

BOOST_AUTO_TEST_CASE(test_count_different_bits_abuse)
{
  const size_t n_loci{3};
  const boost::dynamic_bitset<> a(n_loci);
  const boost::dynamic_bitset<> b(n_loci + 1);
  BOOST_CHECK_THROW(::count_different_bits(a,b), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(test_create_offsping_boost_dynamic_bitset_use)
{
  {
    const size_t n_loci{3};
    const boost::dynamic_bitset<> p(n_loci, 0b001);
    const boost::dynamic_bitset<> q(n_loci, 0b100);
    const boost::dynamic_bitset<> inherit_from_p(n_loci, 0b001);
    const boost::dynamic_bitset<> kid = create_offspring(p, q, inherit_from_p);
    const boost::dynamic_bitset<> kid_should_be(n_loci, 0b101);
    BOOST_CHECK(kid == kid_should_be);
  }
  {
    const size_t n_loci{4};
    const boost::dynamic_bitset<> p(n_loci, 0b0000);
    const boost::dynamic_bitset<> q(n_loci, 0b1111);
    const boost::dynamic_bitset<> inherit_from_p(n_loci, 0b0101);
    const boost::dynamic_bitset<> kid = create_offspring(p, q, inherit_from_p);
    const boost::dynamic_bitset<> kid_should_be(n_loci, 0b1010);
    BOOST_CHECK(kid == kid_should_be);
  }
  {
    const size_t n_loci{4};
    const boost::dynamic_bitset<> p(n_loci, 0b1111);
    const boost::dynamic_bitset<> q(n_loci, 0b0000);
    const boost::dynamic_bitset<> inherit_from_p(n_loci, 0b0101);
    const boost::dynamic_bitset<> kid = create_offspring(p, q, inherit_from_p);
    const boost::dynamic_bitset<> kid_should_be(n_loci, 0b0101);
    BOOST_CHECK(kid == kid_should_be);
  }
}

BOOST_AUTO_TEST_CASE(test_create_offsping_boost_dynamic_bitset_abuse)
{
  {
    //p and q have different sizes
    const size_t n_loci{3};
    const boost::dynamic_bitset<> p(n_loci);
    const boost::dynamic_bitset<> q(n_loci + 1);
    const boost::dynamic_bitset<> inherit_from_p(n_loci, 0b001);
    BOOST_CHECK_THROW(create_offspring(p, q, inherit_from_p), std::invalid_argument);
  }
  {
    //p and inherit_from_p have different sizes
    const size_t n_loci{3};
    const boost::dynamic_bitset<> p(n_loci);
    const boost::dynamic_bitset<> q(n_loci);
    const boost::dynamic_bitset<> inherit_from_p(n_loci + 1);
    BOOST_CHECK_THROW(create_offspring(p, q, inherit_from_p), std::invalid_argument);
  }
}

BOOST_AUTO_TEST_CASE(test_create_offsping_dna_use)
{
  {
    const size_t n_loci{3};
    const dna p("AAA");
    const dna q("CCC");
    const boost::dynamic_bitset<> inherit_from_p(n_loci, 0b001);
    const dna kid = create_offspring(p, q, inherit_from_p);
    const dna kid_should_be("CCA");
    BOOST_CHECK(kid == kid_should_be);
  }
  {
    const size_t n_loci{4};
    const dna p("GGGG");
    const dna q("TTTT");
    const boost::dynamic_bitset<> inherit_from_p(n_loci, 0b0101);
    const dna kid = create_offspring(p, q, inherit_from_p);
    const dna kid_should_be("TGTG");
    BOOST_CHECK(kid == kid_should_be);
  }
  {
    const size_t n_loci{4};
    const dna p("GGGG");
    const dna q("TTTT");
    const boost::dynamic_bitset<> inherit_from_p(n_loci, 0b1010);
    const dna kid = create_offspring(p, q, inherit_from_p);
    const dna kid_should_be("GTGT");
    BOOST_CHECK(kid == kid_should_be);
  }
}

BOOST_AUTO_TEST_CASE(test_create_offsping_dna_abuse)
{
  {
    //p and q have different sizes
    const size_t n_loci{3};
    const dna p("AAA");
    const dna q("CCCC");
    const boost::dynamic_bitset<> inherit_from_p(n_loci);
    BOOST_CHECK_THROW(create_offspring(p, q, inherit_from_p), std::invalid_argument);
  }
  {
    //p and inherit_from_p have different sizes
    const size_t n_loci{3};
    const dna p("AAA");
    const dna q("CCC");
    const boost::dynamic_bitset<> inherit_from_p(n_loci + 1);
    BOOST_CHECK_THROW(create_offspring(p, q, inherit_from_p), std::invalid_argument);
  }
}

BOOST_AUTO_TEST_CASE(test_create_tally)
{
  {
    std::vector<int> v = {};
    std::vector<int> expected = {};
    const auto result = ::create_tally(v);
    BOOST_CHECK(result == expected);
  }
  {
    std::vector<int> v = {0};
    std::vector<int> expected = {1};
    const auto result = ::create_tally(v);
    BOOST_CHECK(result == expected);
  }
  {
    std::vector<int> v = {0,1,1,2,2,2};
    std::vector<int> expected = {1,2,3};
    const auto result = ::create_tally(v);
    BOOST_CHECK(result == expected);
  }
  {
    std::vector<int> v = {0,0,0};
    std::vector<int> expected = {3};
    const auto result = ::create_tally(v);
    BOOST_CHECK(result == expected);
  }
}



BOOST_AUTO_TEST_CASE(test_get_connected_components_ids)
{
  {
    boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> g;
    boost::add_vertex(g);
    const std::vector<int> expected = { 0 };
    const auto result = ::get_connected_components_ids(g);
    BOOST_CHECK(result == expected);
  }
  {
    boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> g;
    boost::add_vertex(g);
    boost::add_vertex(g);
    const std::vector<int> expected = { 0, 1 };
    const auto result = ::get_connected_components_ids(g);
    BOOST_CHECK(result == expected);
  }
  {
    boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> g;
    const auto vd_a = boost::add_vertex(g);
    const auto vd_b = boost::add_vertex(g);
    boost::add_edge(vd_a, vd_b, g);
    const std::vector<int> expected = { 0, 0 };
    const auto result = ::get_connected_components_ids(g);
    BOOST_CHECK(result == expected);
  }
}

BOOST_AUTO_TEST_CASE(test_ribi_get_unique_indices)
{
  std::mt19937 rng_engine(42);
  //Two values should be different
  {
    const std::size_t sz{10};
    const int n{2};
    const auto v = get_unique_indices(sz, n, rng_engine);
    BOOST_CHECK_EQUAL(static_cast<int>(v.size()), 2);
    BOOST_CHECK_NE(v[0], v[1]);
  }
  //If you request 10 values from a container of size 5,
  //you will get 5 values
  {
    const std::size_t sz{5};
    const int n{10};
    BOOST_CHECK_EQUAL(sz, get_unique_indices(sz, n, rng_engine).size());
  }
  //Can request zero values
  {
    const std::size_t sz{10};
    const int n{0};
    BOOST_CHECK(get_unique_indices(sz, n, rng_engine).empty());
  }
  //Can request zero values from empty collection
  {
    const std::size_t sz{0};
    const int n{0};
    BOOST_CHECK(get_unique_indices(sz, n, rng_engine).empty());
  }
}

#pragma GCC diagnostic pop
