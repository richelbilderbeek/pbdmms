#include "elly_per_species_rates.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace elly;

BOOST_AUTO_TEST_CASE(elly_per_species_rates_constructor_and_getters_must_match)
{
  const per_species_rate ana{0.3};
  const per_species_rate clado_is{0.2};
  const per_species_rate clado_main{0.1};
  const per_species_rate ext_is{0.4};
  const per_species_rate ext_main{0.5};
  const per_species_rate mig_to_is{0.6};
  const per_species_rates r(
    ana,
    clado_is,
    clado_main,
    ext_is,
    ext_main,
    mig_to_is
  );
  BOOST_CHECK_EQUAL(r.get_ana(), ana);
  BOOST_CHECK_EQUAL(r.get_clado_is(), clado_is);
  BOOST_CHECK_EQUAL(r.get_clado_main(), clado_main);
  BOOST_CHECK_EQUAL(r.get_ext_is(), ext_is);
  BOOST_CHECK_EQUAL(r.get_ext_main(), ext_main);
  BOOST_CHECK_EQUAL(r.get_mig_to_is(), mig_to_is);
}

BOOST_AUTO_TEST_CASE(elly_per_species_rates_comparisons)
{
  const per_species_rates a = create_test_rates_1();
  const per_species_rates b = create_test_rates_1();
  const per_species_rates c = create_test_rates_2();
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

BOOST_AUTO_TEST_CASE(elly_parameters_operator_stream_out)
{
  const per_species_rates p = create_test_rates_1();
  std::stringstream s;
  s << p;
  BOOST_CHECK(!s.str().empty());
}

BOOST_AUTO_TEST_CASE(elly_parameters_operator_streaming_once)
{
  const per_species_rates a = create_test_rates_1();
  per_species_rates b = create_test_rates_2();
  assert(a != b);
  std::stringstream s;
  s << a;
  s >> b;
  assert(b != create_test_rates_2());
  BOOST_CHECK_EQUAL(a,b);
}

BOOST_AUTO_TEST_CASE(elly_parameters_operator_streaming_twice)
{
  const per_species_rates a = create_test_rates_1();
  const per_species_rates b = create_test_rates_2();
  per_species_rates c = create_test_rates_2();
  per_species_rates d = create_test_rates_1();
  assert(a != b);
  assert(b != d);
  std::stringstream s;
  s << a << ' ' << b;
  s >> c >> d;
  BOOST_CHECK_EQUAL(a,c);
  BOOST_CHECK_EQUAL(b,d);
}

#pragma GCC diagnostic pop

