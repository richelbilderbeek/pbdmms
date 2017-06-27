#include "sado_helper.h"

#include <boost/test/unit_test.hpp>

using namespace sado;

BOOST_AUTO_TEST_CASE(sado_get_unique)
{
  {
    const std::vector<int> v = {};
    const std::vector<int> measured = get_unique(v);
    const std::vector<int> expected = {};
    BOOST_CHECK_EQUAL_COLLECTIONS(
      std::begin(measured), std::end(measured),
      std::begin(expected), std::end(expected));
  }
  {
    const std::vector<int> v = { 1 };
    const std::vector<int> measured = get_unique(v);
    const std::vector<int> expected = { 1 };
    BOOST_CHECK_EQUAL_COLLECTIONS(
      std::begin(measured), std::end(measured),
      std::begin(expected), std::end(expected));
  }
  {
    const std::vector<int> v = { 1, 1 };
    const std::vector<int> measured = get_unique(v);
    const std::vector<int> expected = { 1 };
    BOOST_CHECK_EQUAL_COLLECTIONS(
      std::begin(measured), std::end(measured),
      std::begin(expected), std::end(expected));
  }
  {
    const std::vector<int> v = { 1, 2 };
    const std::vector<int> measured = get_unique(v);
    const std::vector<int> expected = { 1, 2 };
    BOOST_CHECK_EQUAL_COLLECTIONS(
      std::begin(measured), std::end(measured),
      std::begin(expected), std::end(expected));
  }
  {
    const std::vector<int> v = { 2, 1 };
    const std::vector<int> measured = get_unique(v);
    const std::vector<int> expected = { 1, 2 };
    BOOST_CHECK_EQUAL_COLLECTIONS(
      std::begin(measured), std::end(measured),
      std::begin(expected), std::end(expected));
  }
  {
    const std::vector<int> v = { 2, 2 };
    const std::vector<int> measured = get_unique(v);
    const std::vector<int> expected = { 2 };
    BOOST_CHECK_EQUAL_COLLECTIONS(
      std::begin(measured), std::end(measured),
      std::begin(expected), std::end(expected));
  }
  {
    const std::vector<int> v = { 1, 1, 2 };
    const std::vector<int> measured = get_unique(v);
    const std::vector<int> expected = { 1, 2 };
    BOOST_CHECK_EQUAL_COLLECTIONS(
      std::begin(measured), std::end(measured),
      std::begin(expected), std::end(expected));
  }
  {
    const std::vector<int> v = { 1, 2, 2 };
    const std::vector<int> measured = get_unique(v);
    const std::vector<int> expected = { 1, 2 };
    BOOST_CHECK_EQUAL_COLLECTIONS(
      std::begin(measured), std::end(measured),
      std::begin(expected), std::end(expected));
  }
  {
    const std::vector<int> v = { 1, 2, 3 };
    const std::vector<int> measured = get_unique(v);
    const std::vector<int> expected = { 1, 2, 3 };
    BOOST_CHECK_EQUAL_COLLECTIONS(
      std::begin(measured), std::end(measured),
      std::begin(expected), std::end(expected));
  }
  {
    const std::vector<int> v = { 3, 2, 1 };
    const std::vector<int> measured = get_unique(v);
    const std::vector<int> expected = { 1, 2, 3 };
    BOOST_CHECK_EQUAL_COLLECTIONS(
      std::begin(measured), std::end(measured),
      std::begin(expected), std::end(expected));
  }
  {
    const std::vector<int> v = { 3, 3, 1 };
    const std::vector<int> measured = get_unique(v);
    const std::vector<int> expected = { 1, 3 };
    BOOST_CHECK_EQUAL_COLLECTIONS(
      std::begin(measured), std::end(measured),
      std::begin(expected), std::end(expected));
  }
  {
    const std::vector<int> v = { 3, 3, 3 };
    const std::vector<int> measured = get_unique(v);
    const std::vector<int> expected = { 3 };
    BOOST_CHECK_EQUAL_COLLECTIONS(
      std::begin(measured), std::end(measured),
      std::begin(expected), std::end(expected));
  }
}
