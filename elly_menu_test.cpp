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

#ifdef ELLY_DO_RUN_DAISIE
BOOST_AUTO_TEST_CASE(elly_menu_run_profiling)
{
  menu m( { "elly", "--profile" } );
}
#endif //ELLY_DO_RUN_DAISIE

#ifdef ELLY_DO_RUN_DAISIE
BOOST_AUTO_TEST_CASE(elly_menu_run_created_parameters)
{
  menu m( { "elly", "parameters_3.txt" } );
}
#endif //ELLY_DO_RUN_DAISIE



#pragma GCC diagnostic pop

