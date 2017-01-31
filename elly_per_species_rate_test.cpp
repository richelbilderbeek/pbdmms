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

BOOST_AUTO_TEST_CASE(elly_per_species_rate_operator_streaming_once)
{
  const per_species_rate a(3.14);
  per_species_rate b(0.0);
  std::stringstream s;
  s << a;
  s >> b;
  BOOST_CHECK_EQUAL(a,b);
}

BOOST_AUTO_TEST_CASE(elly_per_species_rate_operator_streaming_twice)
{
  const per_species_rate a(3.14);
  const per_species_rate b(42.42);
  per_species_rate c(0.0);
  per_species_rate d(0.0);
  std::stringstream s;
  s << a << ' ' << b;
  s >> c >> d;
  BOOST_CHECK_EQUAL(a,c);
  BOOST_CHECK_EQUAL(b,d);
}

BOOST_AUTO_TEST_CASE(elly_per_species_rate_must_have_unit)
{
  //Correctly formed
  {
    std::stringstream s;
    s
      << "3.14 per species per time unit" << '\n'
    ;
    per_species_rate p(0.0);
    s >> p;
    BOOST_CHECK_CLOSE(p.get(), per_species_rate(3.14).get(), 0.001);
  }
  //First 'per' misspelled
  {
    std::stringstream s;
    s
      << "3.14 peMISSPELLEDr species per time unit" << '\n'
    ;
    per_species_rate p(0.0);
    BOOST_CHECK_THROW(s >> p, std::invalid_argument);
  }
  //'species' misspelled
  {
    std::stringstream s;
    s
      << "3.14 per speMISSPELLEDcies per time unit" << '\n'
    ;
    per_species_rate p(0.0);
    BOOST_CHECK_THROW(s >> p, std::invalid_argument);
  }
  //Second 'per' misspelled
  {
    std::stringstream s;
    s
      << "3.14 per species peMISSPELLEDr time unit" << '\n'
    ;
    per_species_rate p(0.0);
    BOOST_CHECK_THROW(s >> p, std::invalid_argument);
  }
  //'time' misspelled
  {
    std::stringstream s;
    s
      << "3.14 per species per tiMISSPELLEDme unit" << '\n'
    ;
    per_species_rate p(0.0);
    BOOST_CHECK_THROW(s >> p, std::invalid_argument);
  }
  //'unit' misspelled
  {
    std::stringstream s;
    s
      << "3.14 per species per time unMISSPELLEDit" << '\n'
    ;
    per_species_rate p(0.0);
    BOOST_CHECK_THROW(s >> p, std::invalid_argument);
  }

}

#pragma GCC diagnostic pop
