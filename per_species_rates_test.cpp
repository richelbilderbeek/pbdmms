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

#pragma GCC diagnostic pop

