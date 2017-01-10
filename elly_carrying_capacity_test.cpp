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
  //BOOST_CHECK(c <= a);
  //BOOST_CHECK(c <= b);
  BOOST_CHECK(c <= c);

  BOOST_CHECK(a >= a);
  BOOST_CHECK(a >= b);
  //BOOST_CHECK(a >= c);
  BOOST_CHECK(b >= a);
  BOOST_CHECK(b >= b);
  //BOOST_CHECK(b >= c);
  BOOST_CHECK(c >= a);
  BOOST_CHECK(c >= b);
  BOOST_CHECK(c >= c);

  //BOOST_CHECK(a < a);
  //BOOST_CHECK(a < b);
  BOOST_CHECK(a < c);
  //BOOST_CHECK(b < a);
  //BOOST_CHECK(b < b);
  BOOST_CHECK(b < c);
  //BOOST_CHECK(c < a);
  //BOOST_CHECK(c < b);
  //BOOST_CHECK(c < c);

  //BOOST_CHECK(a > a);
  //BOOST_CHECK(a > b);
  //BOOST_CHECK(a > c);
  //BOOST_CHECK(b > a);
  //BOOST_CHECK(b > b);
  //BOOST_CHECK(b > c);
  BOOST_CHECK(c > a);
  BOOST_CHECK(c > b);
  //BOOST_CHECK(c > c);

}




#pragma GCC diagnostic pop
