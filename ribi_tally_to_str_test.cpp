#include "ribi_tally_to_str.h"

#include <sstream>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace ribi;

BOOST_AUTO_TEST_CASE(test_ribi_tally_to_str)
{
  std::map<std::string, int> m;
  assert(m.empty());
  BOOST_CHECK_EQUAL(tally_to_str(m), "");
  m.insert(std::make_pair("A", 42));
  BOOST_CHECK_EQUAL(tally_to_str(m), "A: 42");
  m.insert(std::make_pair("B", 314));
  BOOST_CHECK_EQUAL(tally_to_str(m), "A: 42, B: 314");
}


#pragma GCC diagnostic pop

