#include <fstream>
#include <iostream>
#include <boost/test/unit_test.hpp>
#include "kewe_individual.h"
// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

BOOST_AUTO_TEST_CASE(test_kewe_individual_default_contructed_inidviduals_should_be_identical)
{
  const indiv a;
  const indiv b;
  BOOST_CHECK(a == b);
}

BOOST_AUTO_TEST_CASE(test_kewe_individual_differrent_inidviduals_should_be_different)
{
  const indiv a;
  indiv b;
  b.init(1.0,2.0,3.0);
  BOOST_CHECK(a != b);
}

BOOST_AUTO_TEST_CASE(test_kewe_copy_cnstructor_should_give_identical_individuals)
{
  indiv a;
  indiv b;
  b.init(1.0,2.0,3.0);
  assert(a != b);
  a = b;
  BOOST_CHECK(a == b);
}

BOOST_AUTO_TEST_CASE(test_kewe_inidivual_print)
{
  indiv a;
  BOOST_CHECK_NO_THROW(a.print());
}

#pragma GCC diagnostic pop




