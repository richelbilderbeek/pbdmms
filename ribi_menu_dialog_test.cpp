#include "ribi_menu_dialog.h"

#include "pbd_helper.h"
#include "ribi_parameters.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace ribi;

BOOST_AUTO_TEST_CASE(test_ribi_menu_dialog_zero_args_shows_help)
{
  menu_dialog d;
  d.execute( {} ); //Shows help
}

BOOST_AUTO_TEST_CASE(test_ribi_menu_dialog_one_arg_shows_help)
{
  menu_dialog d;
  d.execute( { "ribi" } ); //Shows help
}

BOOST_AUTO_TEST_CASE(test_ribi_menu_dialog_execute_create)
{
  menu_dialog d;
  const std::string filename{"ribi_test_parameters"};
  d.execute( { "ribi", "--create",  filename} ); //Creates parameter file
  d.execute( { "ribi", filename } ); //Creates parameter file
  pbd::delete_file(filename);
}

BOOST_AUTO_TEST_CASE(test_ribi_menu_dialog_execute_profiling)
{
  //Don't! This is supposed to be run in release mode.
  //menu_dialog d;
  //d.execute( { "ribi", "--profiling" } ); //Runs demo
}

BOOST_AUTO_TEST_CASE(test_ribi_menu_dialog_execute_test)
{
  #ifdef FIX_ISSUE_41
  menu_dialog d;
  d.execute( { "ribi", "--test" } ); //Runs test
  #endif // FIX_ISSUE_41
}

BOOST_AUTO_TEST_CASE(test_ribi_menu_dialog_execute_with_unimplemented_parameters)
{
  menu_dialog d;
  d.execute( { "ribi", "--do_something_unimplemented" } ); //Shows help
}

BOOST_AUTO_TEST_CASE(test_ribi_menu_dialog_execute_run)
{
  #ifdef FIX_ISSUE_41
  const std::string filename{"test_ribi_menu_dialog_execute_run.txt"};
  const auto p = create_test_parameters_1();
  save_parameters(p, filename);
  menu_dialog d;
  d.execute( { "ribi", filename } ); //Runs it
  #endif // FIX_ISSUE_41
}

#pragma GCC diagnostic pop

