#include "distancer_dna.h"
#include <fstream>
#include <iostream>
#include <boost/test/unit_test.hpp>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

BOOST_AUTO_TEST_CASE(test_dna)
{
  BOOST_CHECK_NO_THROW(dna("A"));
  BOOST_CHECK_NO_THROW(dna(""));
  BOOST_CHECK_THROW(dna("X"), std::invalid_argument);
  const dna a("A");
  const dna b("A");
  BOOST_CHECK(a == b);
}

#pragma GCC diagnostic pop



