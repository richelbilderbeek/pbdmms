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

#pragma GCC diagnostic pop
