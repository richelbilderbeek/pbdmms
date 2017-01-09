#include "elly_per_species_rate.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace elly;

BOOST_AUTO_TEST_CASE(elly_per_species_rate_constructor_and_get_must_match)
{
  const double r{3.14};
  const per_species_rate p(r);
  BOOST_CHECK_EQUAL(p.get(), r);
}

BOOST_AUTO_TEST_CASE(elly_cannot_create_negative_per_species_rate)
{
  BOOST_CHECK_THROW(per_species_rate(-1.2), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(elly_per_species_rate_operator_equals)
{
  const per_species_rate a(3.14);
  const per_species_rate b(3.14);
  const per_species_rate c(42.42);
  BOOST_CHECK(a == a);
  BOOST_CHECK(a == b);
  BOOST_CHECK(a != c);
  BOOST_CHECK(b == a);
  BOOST_CHECK(b == b);
  BOOST_CHECK(b != c);
  BOOST_CHECK(c != a);
  BOOST_CHECK(c != b);
  BOOST_CHECK(c == c);
}

BOOST_AUTO_TEST_CASE(elly_per_species_rate_operator_stream_out)
{
  const per_species_rate a(3.14);
  std::stringstream s;
  s << a;
  BOOST_CHECK(!s.str().empty());
}

#pragma GCC diagnostic pop
