#include "elly_rate.h"


// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace elly;

BOOST_AUTO_TEST_CASE(elly_rate_constructor_and_getter_must_match)
{
  const double r{3.14};
  const rate t(r);
  BOOST_CHECK_EQUAL(r, t.get());
}

BOOST_AUTO_TEST_CASE(elly_rate_comparison_operators)
{
  const rate a(3.14);
  const rate b(3.14);
  const rate c(42.0);
  BOOST_CHECK(a == a);
  BOOST_CHECK(a == b);
  BOOST_CHECK(a != c);
  BOOST_CHECK(b == a);
  BOOST_CHECK(b == b);
  BOOST_CHECK(b != c);
  BOOST_CHECK(c != a);
  BOOST_CHECK(c != b);
  BOOST_CHECK(c == c);

  BOOST_CHECK(a <  c);
  BOOST_CHECK(a <= a);
  BOOST_CHECK(a <= b);
  BOOST_CHECK(a <= c);
}


#pragma GCC diagnostic pop


