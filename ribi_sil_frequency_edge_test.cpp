#include "ribi_sil_frequency_edge.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace ribi;

BOOST_AUTO_TEST_CASE(test_sil_frequency_edge_use)
{
  const int n_timesteps{42};
  sil_frequency_edge e(n_timesteps);
  BOOST_CHECK_EQUAL(e.get_n_timesteps(), n_timesteps);
}

BOOST_AUTO_TEST_CASE(test_sil_frequency_edge_abuse)
{
  BOOST_CHECK_THROW(sil_frequency_edge(-42), std::invalid_argument);
}

#pragma GCC diagnostic pop

