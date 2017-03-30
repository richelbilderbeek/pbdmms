#include "sado_mate_graph.h"

#include <boost/test/unit_test.hpp>

using namespace sado;


BOOST_AUTO_TEST_CASE(sado_create_test_mate_graph1)
{
  const auto g = create_test_mate_graph1();
  BOOST_CHECK_EQUAL(boost::num_vertices(g), 0);
  BOOST_CHECK_EQUAL(boost::num_edges(g), 0);
}

BOOST_AUTO_TEST_CASE(sado_create_test_mate_graph2)
{
  const auto g = create_test_mate_graph2();
  BOOST_CHECK_EQUAL(boost::num_vertices(g), 1);
  BOOST_CHECK_EQUAL(boost::num_edges(g), 0);
}

BOOST_AUTO_TEST_CASE(sado_create_test_mate_graph3)
{
  const auto g = create_test_mate_graph3();
  BOOST_CHECK_EQUAL(boost::num_vertices(g), 2);
  BOOST_CHECK_EQUAL(boost::num_edges(g), 1);
}

BOOST_AUTO_TEST_CASE(sado_create_test_mate_graph4)
{
  const auto g = create_test_mate_graph4();
  BOOST_CHECK_EQUAL(boost::num_vertices(g), 2);
  BOOST_CHECK_EQUAL(boost::num_edges(g), 0);
}

BOOST_AUTO_TEST_CASE(sado_seperate_individuals_by_id)
{
  {
    const auto g = create_test_mate_graph1();
    assert(boost::num_vertices(g) == 0);
    const auto m = seperate_individuals_by_id( {}, g );
    BOOST_CHECK(m.empty());
  }
  {
    const auto g = create_test_mate_graph2();
    assert(boost::num_vertices(g) == 1);
    const auto m = seperate_individuals_by_id( { 0 }, g );
    BOOST_CHECK_EQUAL((*m.find(0)).second.size(), 1);
  }
  {
    const auto g = create_test_mate_graph2();
    assert(boost::num_vertices(g) == 1);
    const auto m = seperate_individuals_by_id( { 1 }, g );
    BOOST_CHECK_EQUAL((*m.find(1)).second.size(), 1);
  }
  {
    const auto g = create_test_mate_graph3();
    assert(boost::num_vertices(g) == 2);
    const auto m = seperate_individuals_by_id( { 0, 1 }, g );
    BOOST_CHECK_EQUAL((*m.find(0)).second.size(), 1);
    BOOST_CHECK_EQUAL((*m.find(1)).second.size(), 1);
  }
  {
    const auto g = create_test_mate_graph3();
    assert(boost::num_vertices(g) == 2);
    const auto m = seperate_individuals_by_id( { 0, 0 }, g );
    BOOST_CHECK_EQUAL((*m.find(0)).second.size(), 2);
  }
  {
    const auto g = create_test_mate_graph3();
    BOOST_CHECK_THROW(
      seperate_individuals_by_id( {}, g ),
      std::invalid_argument
    );
  }
}
