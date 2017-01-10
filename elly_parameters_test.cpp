#include "elly_parameters.h"

#include <sstream>
#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string/split.hpp>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

using namespace elly;

BOOST_AUTO_TEST_CASE(elly_parameters_settings)
{
  const parameters a = create_parameters_set1();
  const parameters b = create_parameters_set1();
  BOOST_CHECK(a.get_ana_rate() == b.get_ana_rate());

}

BOOST_AUTO_TEST_CASE(elly_all_parameters_settings)
{
  const parameters a = create_parameters_set1();
  const parameters b = create_parameters_set1();
  const parameters c = create_parameters_set2();
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

BOOST_AUTO_TEST_CASE(elly_create_parameters_sets)
{
  BOOST_CHECK_NO_THROW(create_parameters_set1());
  BOOST_CHECK_NO_THROW(create_parameters_set2());
  BOOST_CHECK_NO_THROW(create_parameters_set3());
  BOOST_CHECK_NO_THROW(create_profiling_parameters());
}

BOOST_AUTO_TEST_CASE(elly_parameters_operator_stream_out)
{
  const auto p = create_parameters_set1();
  std::stringstream s;
  s << p;
  BOOST_CHECK(!s.str().empty());
}

#pragma GCC diagnostic pop
