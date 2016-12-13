#include "jaan_parameters.h"

#include <boost/test/unit_test.hpp>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

BOOST_AUTO_TEST_CASE(jaan_parameters_use) {
  Parameters p;
  p.set_gEnd(100);
  BOOST_CHECK(p.get_gEnd() == 100);
  p.set_popSize(100);
  BOOST_CHECK(p.get_popSize() == 100);
  p.set_nPrefGenes(100);
  BOOST_CHECK(p.get_nPrefGenes() == 100);
  p.set_nTrtGenes(100);
  BOOST_CHECK(p.get_nTrtGenes() == 100);
  p.set_traitCost(100);
  BOOST_CHECK(p.get_traitCost() == 100);
  p.set_pOpt(100);
  BOOST_CHECK(p.get_pOpt() == 100);
  p.set_tOpt(100);
  BOOST_CHECK(p.get_tOpt() == 100);
  p.set_deltap(100);
  BOOST_CHECK(p.get_deltap() == 100);
  p.set_deltat(100);
  BOOST_CHECK(p.get_deltat() == 100);
  p.set_mu(100);
  BOOST_CHECK(p.get_mu() == 100);
}

BOOST_AUTO_TEST_CASE(jaan_parameters_abuse) {
  Parameters p;
  BOOST_CHECK_THROW(
    p.set_gEnd(-100),
    std::invalid_argument
  );
  BOOST_CHECK_THROW(
    p.set_popSize(-100),
    std::invalid_argument
  );
  BOOST_CHECK_THROW(
    p.set_nPrefGenes(-100),
    std::invalid_argument
  );
  BOOST_CHECK_THROW(
    p.set_nTrtGenes(-100),
    std::invalid_argument
  );
}

#pragma GCC diagnostic pop



