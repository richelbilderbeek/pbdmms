#include "ribi_jkr_adapters.h"
#include "jkr_experiment.h"
#include <fstream>
#include <iostream>
#include "ribi_helper.h"
#include "ribi_results.h"
#include "ribi_parameters.h"
#include "ribi_simulation.h"
#include "pbd_helper.h"
#include "pbd_ltt.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace ribi;

BOOST_AUTO_TEST_CASE(test_ribi_jkr_do_experiment_should_produce_ltt_file)
{
  const auto p = create_test_parameters_1();

  //Remove output if already present
  if (pbd::is_regular_file(p.get_ltt_plot_filename()))
  {
    pbd::delete_file(p.get_ltt_plot_filename());
  }

  assert(!pbd::is_regular_file(p.get_ltt_plot_filename()));
  jkr::do_experiment<parameters, simulation, results>(p);
  BOOST_CHECK(pbd::is_regular_file(p.get_ltt_plot_filename()));

  //Clean up
  if (pbd::is_regular_file(p.get_ltt_plot_filename()))
  {
    pbd::delete_file(p.get_ltt_plot_filename());
  }
}

BOOST_AUTO_TEST_CASE(test_ribi_jkr_do_experiment_should_produce_ltt_file_of_right_size)
{
  const auto p = create_test_parameters_1();

  //Remove output if already present
  if (pbd::is_regular_file(p.get_ltt_plot_filename()))
  {
    pbd::delete_file(p.get_ltt_plot_filename());
  }

  assert(!pbd::is_regular_file(p.get_ltt_plot_filename()));
  jkr::do_experiment<parameters, simulation, results>(p);
  assert(pbd::is_regular_file(p.get_ltt_plot_filename()));
  const pbd::ltt my_ltt = pbd::load_ltt_from_csv(p.get_ltt_plot_filename());
  BOOST_CHECK_EQUAL(my_ltt.size(), p.get_n_generations());

  //Clean up
  if (pbd::is_regular_file(p.get_ltt_plot_filename()))
  {
    pbd::delete_file(p.get_ltt_plot_filename());
  }
}

#pragma GCC diagnostic pop

