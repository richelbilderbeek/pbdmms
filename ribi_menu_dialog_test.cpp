#include "ribi_menu_dialog.h"

#include "ribi_parameters.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace ribi;

BOOST_AUTO_TEST_CASE(test_ribi_menu_dialog_show_help)
{
  menu_dialog d;
  d.execute( {} ); //Shows help
}

BOOST_AUTO_TEST_CASE(test_ribi_menu_dialog_execute_demo)
{
  menu_dialog d;
  d.execute( { "ribi", "--demo" } ); //Runs demo
}

BOOST_AUTO_TEST_CASE(test_ribi_menu_dialog_execute_with_unimplemented_parameters)
{
  menu_dialog d;
  d.execute( { "ribi", "--do_something_unimplemented" } ); //Shows help
}

BOOST_AUTO_TEST_CASE(test_ribi_menu_dialog_execute_run)
{
  const std::string filename{"test_ribi_menu_dialog_execute_run.txt"};
  const auto p = create_test_parameters_1();
  save_parameters(p, filename);
  menu_dialog d;
  d.execute( { "ribi", filename } ); //Runs it
}

#pragma GCC diagnostic pop

