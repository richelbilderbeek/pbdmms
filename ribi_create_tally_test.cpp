#include "ribi_create_tally.h"

#include <iostream>
#include <fstream>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace ribi;

BOOST_AUTO_TEST_CASE(test_ribi_create_tally)
{
  {
    const std::vector<std::string> v = {};
    const std::map<std::string, int> expected = {};
    const auto result = ::create_tally(v);
    BOOST_CHECK(result == expected);
  }
  {
    const std::vector<std::string> v = { "A" };
    const std::map<std::string, int> expected = { {"A", 1} };
    const auto result = ::create_tally(v);
    BOOST_CHECK(result == expected);
  }
  {
    const std::vector<std::string> v = { "A", "A" };
    const std::map<std::string, int> expected = { {"A", 2} };
    const auto result = ::create_tally(v);
    BOOST_CHECK(result == expected);
  }
  {
    const std::vector<std::string> v = { "A", "B" };
    const std::map<std::string, int> expected = { {"A", 1}, { "B", 1} };
    const auto result = ::create_tally(v);
    BOOST_CHECK(result == expected);
  }
  {
    const std::vector<std::string> v = { "A", "A", "B" };
    const std::map<std::string, int> expected = { {"A", 2}, {"B", 1} };
    const auto result = ::create_tally(v);
    BOOST_CHECK(result == expected);
  }
  {
    const std::vector<std::string> v = { "B", "A", "A" };
    const std::map<std::string, int> expected = { {"A", 2}, {"B", 1} };
    const auto result = ::create_tally(v);
    BOOST_CHECK(result == expected);
  }
}

#pragma GCC diagnostic pop
