#include<iostream>
#include "elly_parameters.h"
#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string/split.hpp>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

using namespace elly;

BOOST_AUTO_TEST_CASE(elly_parameters_settings)
{
  const parameters a = create_parameters_set1();
  const parameters b = create_parameters_set1();
  BOOST_CHECK(a.get_ana_rate() == b.get_ana_rate());

}

BOOST_AUTO_TEST_CASE(elly_all_parameters_settings)
{
  const parameters a = create_parameters_set1();
  const parameters b = create_parameters_set1();
  BOOST_CHECK(a == b);
}

#pragma GCC diagnostic pop
