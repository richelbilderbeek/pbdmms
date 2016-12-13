#include<iostream>
#include "elly_rates.h"

#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string/split.hpp>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

using namespace elly;

BOOST_AUTO_TEST_CASE(test_vector_length)
{
  const rates r;
  std::vector<double> ratesvec = to_ratesvector(r);
  BOOST_CHECK(ratesvec.size() == 11);
}

#pragma GCC diagnostic pop
