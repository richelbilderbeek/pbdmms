#include "elly_menu.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace elly;

BOOST_AUTO_TEST_CASE(elly_menu_show_help)
{
  menu m( { "elly" } );
}

BOOST_AUTO_TEST_CASE(elly_menu_create_test_files)
{
  menu m( { "elly", "--create" } );
}

#pragma GCC diagnostic pop

