#include "daic_input_row.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace daic;

BOOST_AUTO_TEST_CASE(daic_input_row)
{
  {
    const std::string s{"a\tb\t\tc"};
    assert(std::count(std::begin(s), std::end(s), '\t') == 3);
  }
  {
    const std::string s = create_input_header();
    BOOST_CHECK_EQUAL(std::count(s.begin(), s.end(), '\t'), 3);
  }
  {
    std::stringstream s;
    const input_row r("platypus", species_status::endemic, 0, std::vector<double>{1.2, 0.98, 0.54});
    s << r;
    const std::string t{s.str()};
  }
}
#pragma GCC diagnostic pop
