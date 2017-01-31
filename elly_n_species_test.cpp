#include "elly_n_species.h"

#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string/split.hpp>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

using namespace elly;

BOOST_AUTO_TEST_CASE(test_n_species_constructor_and_get_must_match)
{
  const int n{42};
  const n_species p(n);
  BOOST_CHECK_EQUAL(p.get(), n);

  BOOST_CHECK_THROW(n_species(-42), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(test_n_species_comparison_operators)
{
  const n_species a(42);
  const n_species b(42);
  const n_species c(314);
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

BOOST_AUTO_TEST_CASE(elly_n_species_operator_stream_out)
{
  const n_species p(1);
  std::stringstream s;
  s << p;
  BOOST_CHECK(!s.str().empty());
}

BOOST_AUTO_TEST_CASE(elly_n_species_operator_streaming_once)
{
  const n_species a(1);
  n_species b(2);
  assert(a != b);
  std::stringstream s;
  s << a;
  s >> b;
  assert(b != n_species(2));
  BOOST_CHECK_EQUAL(a,b);
}

BOOST_AUTO_TEST_CASE(elly_n_species_operator_streaming_twice)
{
  const n_species a(1);
  const n_species b(2);
  n_species c(2);
  n_species d(1);
  assert(a != b);
  assert(b != d);
  std::stringstream s;
  s << a << ' ' << b;
  s >> c >> d;
  BOOST_CHECK_EQUAL(a,c);
  BOOST_CHECK_EQUAL(b,d);
}

#pragma GCC diagnostic pop
