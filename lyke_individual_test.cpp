#include <cassert>
#include <iostream>
#include <fstream>
#include "lyke_random.h"
#include "lyke_individual.h"
#include "lyke_simulation.h"
#include "lyke_utils.h"
#include "pbd_helper.h"
#include <QFile>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(lyke_default_constructed_individuals_are_identical)
{
  #ifdef REALLY_INTERESTED
  const Individual a;
  const Individual b;
  BOOST_CHECK_EQUAL(a, b);
  #endif // REALLY_INTERESTED
}

BOOST_AUTO_TEST_CASE(lyke_default_copied_individuals_are_equal)
{
  Individual a;
  a.ugly();
  Individual b(a);
  BOOST_CHECK_EQUAL(a, b);
}

BOOST_AUTO_TEST_CASE(lyke_calculate_fertilization_efficiency)
{
  #ifdef REALLY_INTERESTED
  //Clones attract
  {
    const Individual a;
    const Individual b(a);
    BOOST_CHECK_EQUAL(calculate_fertilization_efficiency(a,b), 1.0);
    BOOST_CHECK_EQUAL(calculate_fertilization_efficiency(b,a), 1.0);
  }
  #endif // REALLY_INTERESTED
}

BOOST_AUTO_TEST_CASE(lyke_create_opposite)
{
  #ifdef REALLY_INTERESTED
  //Opposites think they are ugly
  const Individual a;
  const Individual b = create_opposite(a);
  BOOST_CHECK(calculate_fertilization_efficiency(a,b) < 0.01);
  BOOST_CHECK(calculate_fertilization_efficiency(b,a) < 0.01);
  #endif // REALLY_INTERESTED
}

BOOST_AUTO_TEST_CASE(lyke_ugly_ugly_individuals_are_not_ugly)
{
  #ifdef REALLY_INTERESTED
  Individual a;
  a.ugly();
  Individual b(a);
  assert(a == b);
  assert(b.match(&a) == 1.0); //Ugly individuals match
  //Now make b ugly agin
  b.ugly();
  BOOST_CHECK(b.match(&a) < 0.99);
  #endif // REALLY_INTERESTED
}

BOOST_AUTO_TEST_CASE(lyke_individual_stream_out_operator)
{
  #ifdef REALLY_INTERESTED
  const Individual i;
  std::stringstream s;
  s << i;
  BOOST_CHECK(!s.str().empty());
  BOOST_CHECK(s.str() != "STUB");
  #endif //REALLY_INTERESTED
}

#pragma GCC diagnostic pop


