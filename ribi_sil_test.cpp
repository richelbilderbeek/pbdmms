#include "ribi_sil.h"

#include <boost/dynamic_bitset.hpp>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace ribi;

BOOST_AUTO_TEST_CASE(test_ribi_create_sil)
{
  {
    const sil expected(3, 0b010);
    const std::string s{"010"};
    const sil result = create_sil(s);
    BOOST_CHECK(expected == result);
  }
  {
    const sil expected(0, 0);
    const std::string s{""};
    const sil result = create_sil(s);
    BOOST_CHECK(expected == result);
  }
}
