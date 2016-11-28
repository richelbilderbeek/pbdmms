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
  const parameters p(10,42,0.5,10,6);
  jkr::do_experiment<
    jobo::parameters,
    jobo::simulation,
    jobo::results
  >(p);
}

BOOST_AUTO_TEST_CASE(test_jobo_create_next_population)
{
  const parameters p(10,42,0.5,10,6);
  simulation s = create_simulation(p);
  std::mt19937 rng_engine(get_rng_seed(p));
  vector <individual> test_population = create_next_population(s,rng_engine);
  BOOST_CHECK (test_population.size() > 0);
}

BOOST_AUTO_TEST_CASE(test_jobo_create_ltt_plot_filename)
{
  const parameters d(10,42,0.5,10,6);
  std::string filename = get_ltt_plot_filename(d);
  std::string e ("jobo abcdef,10,6,0.5,10,42");
  BOOST_CHECK (filename == e);
}

#pragma GCC diagnostic pop


