#include "jkr_experiment.h"
#include "jobo_jkr_adapters.h"
#include "jobo_parameters.h"
#include "jobo_simulation.h"
#include "jobo_results.h"
#include <cassert>
#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <boost/test/unit_test.hpp>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

using namespace jobo;

BOOST_AUTO_TEST_CASE(test_jobo_jkr_adapters_test)
{
  const vector<individual> individuals{10, individual("aBCdEfG")};
  const parameters a(1,38,0.5,10,individuals);

  jkr::do_experiment<
    jobo::parameters,
    jobo::simulation,
    jobo::results
  >(a);
}

#pragma GCC diagnostic pop


