#include "elly_experiment.h"
#include "daic_helper.h"
#include "daic_run.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace elly;

BOOST_AUTO_TEST_CASE(elly_experiment_default_construction)
{
  const elly::parameters p = elly::create_parameters_set1();
  elly::experiment e(p);
  BOOST_CHECK(is_empty(e.get_simulation_results()));
  BOOST_CHECK(is_empty(e.get_input_ideal()));
  BOOST_CHECK(is_empty(e.get_input_reality()));
  BOOST_CHECK(is_empty(e.get_output_ideal()));
  BOOST_CHECK(is_empty(e.get_output_reality()));
}

BOOST_AUTO_TEST_CASE(elly_experiment_run_sim)
{
  const elly::parameters p = elly::create_parameters_set1();
  elly::experiment e(p);
  e.run_sim();
  BOOST_CHECK(!is_empty(e.get_simulation_results()));
  BOOST_CHECK(is_empty(e.get_input_ideal()));
  BOOST_CHECK(is_empty(e.get_input_reality()));
  BOOST_CHECK(is_empty(e.get_output_ideal()));
  BOOST_CHECK(is_empty(e.get_output_reality()));
}

BOOST_AUTO_TEST_CASE(elly_experiment_create_daisie_input)
{
  const elly::parameters p = elly::create_parameters_set2();
  elly::experiment e(p);
  e.run_sim();
  e.create_daisie_input();
  BOOST_CHECK(!is_empty(e.get_simulation_results()));
  BOOST_CHECK(!is_empty(e.get_input_ideal()));
  BOOST_CHECK(!is_empty(e.get_input_reality()));
  BOOST_CHECK(is_empty(e.get_output_ideal()));
  BOOST_CHECK(is_empty(e.get_output_reality()));
}

#ifdef REALLY_RUN_DAISIE
BOOST_AUTO_TEST_CASE(elly_experiment_run_daisie)
{
  const auto argc = boost::unit_test::framework::master_test_suite().argc;
  const auto argv = boost::unit_test::framework::master_test_suite().argv;
  assert(argc >= 1);
  daic::set_r_working_directory(daic::get_path(argv[0]));
  const elly::parameters p = elly::create_parameters_set2();
  elly::experiment e(p);
  e.run_sim();
  e.create_daisie_input();
  e.run_daisie();
  BOOST_CHECK(!is_empty(e.get_simulation_results()));
  BOOST_CHECK(!is_empty(e.get_input_ideal()));
  BOOST_CHECK(!is_empty(e.get_input_reality()));
  BOOST_CHECK(!is_empty(e.get_output_ideal()));
  BOOST_CHECK(!is_empty(e.get_output_reality()));
}
#endif //REALLY_RUN_DAISIE

#pragma GCC diagnostic pop

