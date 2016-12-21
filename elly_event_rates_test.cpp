#include "elly_event_rates.h"

#include <cmath>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace elly;

BOOST_AUTO_TEST_CASE(elly_calc_anagenesis)
{
  {
    const double anagenesis_rate{0.5};
    const int n_global_species{20};
    const double expected{10.0};
    const double measured{calc_anagenesis(anagenesis_rate, n_global_species)};
    BOOST_CHECK(std::abs(expected - measured) < 0.001);
  }
  {
    const double anagenesis_rate{0.25};
    const int n_global_species{80};
    const double expected{20.0};
    const double measured{calc_anagenesis(anagenesis_rate, n_global_species)};
    BOOST_CHECK(std::abs(expected - measured) < 0.001);
  }
}

BOOST_AUTO_TEST_CASE(elly_calc_cladogenesis_main)
{
  {
    const double clad_rate_main{0.02};
    const int carrying_cap_main{}
  }
}

#pragma GCC diagnostic pop
