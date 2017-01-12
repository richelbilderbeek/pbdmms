#include "elly_carrying_capacity.h"

#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string/split.hpp>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

using namespace elly;

BOOST_AUTO_TEST_CASE(test_carrying_capacity_constructor_and_get_must_match)
{
  const int n{42};
  const carrying_capacity p(n);
  BOOST_CHECK_EQUAL(p.get(), n);

  BOOST_CHECK_THROW(carrying_capacity(-42), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(test_carrying_capacity_comparison_operators)
{
  const carrying_capacity a(42);
  const carrying_capacity b(42);
  const carrying_capacity c(314);
  BOOST_CHECK(a == a);
  BOOST_CHECK(a == b);
  BOOST_CHECK(a != c);
  BOOST_CHECK(b == a);
  BOOST_CHECK(b == b);
  BOOST_CHECK(b != c);
  BOOST_CHECK(c != a);
  BOOST_CHECK(c != b);
  BOOST_CHECK(c == c);

  BOOST_CHECK(a <= a);
  BOOST_CHECK(a <= b);
  BOOST_CHECK(a <= c);
  BOOST_CHECK(b <= a);
  BOOST_CHECK(b <= b);
  BOOST_CHECK(b <= c);
  BOOST_CHECK(c <= c);

  BOOST_CHECK(a >= a);
  BOOST_CHECK(a >= b);
  BOOST_CHECK(b >= a);
  BOOST_CHECK(b >= b);
  BOOST_CHECK(c >= a);
  BOOST_CHECK(c >= b);
  BOOST_CHECK(c >= c);

  BOOST_CHECK(a < c);
  BOOST_CHECK(b < c);

  BOOST_CHECK(c > a);
  BOOST_CHECK(c > b);
}

BOOST_AUTO_TEST_CASE(elly_carrying_capacity_operator_stream_out)
{
  const carrying_capacity p(1);
  std::stringstream s;
  s << p;
  BOOST_CHECK(!s.str().empty());
}

BOOST_AUTO_TEST_CASE(elly_carrying_capacity_operator_streaming_once)
{
  const carrying_capacity a(1);
  carrying_capacity b(2);
  assert(a != b);
  std::stringstream s;
  s << a;
  s >> b;
  assert(b != carrying_capacity(2));
  BOOST_CHECK_EQUAL(a,b);
}

BOOST_AUTO_TEST_CASE(elly_carrying_capacity_operator_streaming_twice)
{
  const carrying_capacity a(1);
  const carrying_capacity b(2);
  carrying_capacity c(2);
  carrying_capacity d(1);
  assert(a != b);
  assert(b != d);
  std::stringstream s;
  s << a << ' ' << b;
  s >> c >> d;
  BOOST_CHECK_EQUAL(a,c);
  BOOST_CHECK_EQUAL(b,d);
}

#pragma GCC diagnostic pop
