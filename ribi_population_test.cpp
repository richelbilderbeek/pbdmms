#include "ribi_population.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace ribi;

BOOST_AUTO_TEST_CASE(test_tally_sils)
{
  population p;
  p.push_back(individual(individual::pin_t("AAA"), individual::sil_t(3, 0b001)));
  const auto m = tally_sils(p);
  BOOST_CHECK_EQUAL(m.size(), 1);
}

#pragma GCC diagnostic pop
