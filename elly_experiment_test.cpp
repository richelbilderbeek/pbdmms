#include "elly_experiment.h"


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

BOOST_AUTO_TEST_CASE(elly_experiment_run_daisie)
{
  //daic::set_r_working_directory(daic::get_path(argv[0]));
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

#pragma GCC diagnostic pop








