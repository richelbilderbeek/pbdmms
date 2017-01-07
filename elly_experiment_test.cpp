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
  BOOST_CHECK(is_empty(e.get_di_with_main_ext()));
  BOOST_CHECK(is_empty(e.get_di_without_main_ext()));
  BOOST_CHECK(is_empty(e.get_do_with_main_ext()));
  BOOST_CHECK(is_empty(e.get_do_without_main_ext()));
}

BOOST_AUTO_TEST_CASE(elly_experiment_run_sim)
{
  //daic::set_r_working_directory(daic::get_path(argv[0]));
  const elly::parameters p = elly::create_parameters_set1();
  elly::experiment e(p);
  e.run_sim();
  BOOST_CHECK(!is_empty(e.get_simulation_results()));
  BOOST_CHECK(is_empty(e.get_di_with_main_ext()));
  BOOST_CHECK(is_empty(e.get_di_without_main_ext()));
  BOOST_CHECK(is_empty(e.get_do_with_main_ext()));
  BOOST_CHECK(is_empty(e.get_do_without_main_ext()));
}

BOOST_AUTO_TEST_CASE(elly_experiment_create_daisie_input)
{
  //daic::set_r_working_directory(daic::get_path(argv[0]));
  const elly::parameters p = elly::create_parameters_set1();
  elly::experiment e(p);
  e.run_sim();
  e.create_daisie_input();
  BOOST_CHECK(!is_empty(e.get_simulation_results()));
  BOOST_CHECK(!is_empty(e.get_di_with_main_ext()));
  BOOST_CHECK(!is_empty(e.get_di_without_main_ext()));
  BOOST_CHECK(is_empty(e.get_do_with_main_ext()));
  BOOST_CHECK(is_empty(e.get_do_without_main_ext()));
}

#ifdef HAS_FIXED_180
BOOST_AUTO_TEST_CASE(elly_experiment_run_daisie)
{
  const auto argc = boost::unit_test::framework::master_test_suite().argc;
  const auto argv = boost::unit_test::framework::master_test_suite().argv;
  assert(argc >= 1);
  daic::set_r_working_directory(daic::get_path(argv[0]));
  const elly::parameters p = elly::create_parameters_set1();
  elly::experiment e(p);
  e.run_sim();
  e.create_daisie_input();
  e.run_daisie();
  BOOST_CHECK(!is_empty(e.get_simulation_results()));
  BOOST_CHECK(!is_empty(e.get_di_with_main_ext()));
  BOOST_CHECK(!is_empty(e.get_di_without_main_ext()));
  BOOST_CHECK(!is_empty(e.get_do_with_main_ext()));
  BOOST_CHECK(!is_empty(e.get_do_without_main_ext()));
}
#endif // HAS_FIXED_180

#pragma GCC diagnostic pop

