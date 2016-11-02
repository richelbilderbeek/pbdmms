#include <fstream>
#include <iostream>
#include <boost/test/unit_test.hpp>
#include "kewe_individual.h"
#include "kewe_parameters.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

BOOST_AUTO_TEST_CASE(test_kewe_individual_default_contructed_inidviduals_should_be_identical)
{
  kewe_parameters p;
  const indiv a(p);
  const indiv b(p);
  BOOST_CHECK(a == b);
}

BOOST_AUTO_TEST_CASE(test_kewe_individual_differrent_inidviduals_should_be_different)
{
  kewe_parameters p;
  const indiv a(p);
  indiv b(p);
  b.init(p);
  BOOST_CHECK(a != b);
}

BOOST_AUTO_TEST_CASE(test_kewe_copy_cnstructor_should_give_identical_individuals)
{
  kewe_parameters p;
  indiv a(p);
  indiv b(p);
  b.init(p);
  assert(a != b);
  a = b;
  BOOST_CHECK(a == b);
}

BOOST_AUTO_TEST_CASE(test_kewe_inidivual_print)
{
  kewe_parameters p;
  indiv a(p);
  BOOST_CHECK_NO_THROW(a.print());
}

#pragma GCC diagnostic pop




