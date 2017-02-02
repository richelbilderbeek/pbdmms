#include "pbd_menu.h"

#include "pbd_helper.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace pbd;

BOOST_AUTO_TEST_CASE(pbd_menu_use_without_arguments_shows_help)
{
  menu m( {} );
  BOOST_CHECK_NO_THROW(m.run());
}

BOOST_AUTO_TEST_CASE(pbd_menu_use_version_shows_version)
{
  menu m( { "--version" } );
  BOOST_CHECK_NO_THROW(m.run());
}

BOOST_AUTO_TEST_CASE(pbd_menu_use_create_does_create_files)
{
  const std::string filename{"pbd.txt"};
  if (is_regular_file(filename)) { delete_file(filename); }
  menu m( { "--create" } );
  m.run();
  BOOST_CHECK(is_regular_file(filename));
}

#pragma GCC diagnostic pop



