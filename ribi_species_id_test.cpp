#include "ribi_species_id.h"

#include <sstream>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace ribi;

BOOST_AUTO_TEST_CASE(test_species_id_comparison)
{
  const species_id a;
  const species_id a_too{a};
  const species_id b;
  const species_id b_too{b};
  BOOST_CHECK(a == a_too);
  BOOST_CHECK(b == b_too);
  BOOST_CHECK(a != b_too);
  BOOST_CHECK(b != a_too);
}

BOOST_AUTO_TEST_CASE(test_species_id_stream)
{
  const species_id a;
  std::stringstream s;
  s << a;
  BOOST_CHECK(!s.str().empty());
}

#pragma GCC diagnostic pop
