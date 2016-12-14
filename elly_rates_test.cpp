#include<iostream>
#include "elly_rates.h"

#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string/split.hpp>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

using namespace elly;

BOOST_AUTO_TEST_CASE(test_rates_vector)
{
  const rates r;
  std::vector<double> ratesvec = to_ratesvector(r);
  BOOST_CHECK(ratesvec.size() == 11);

  BOOST_CHECK(r.get_mclad() == ratesvec[0]);
  BOOST_CHECK(r.get_mext() == ratesvec[1]);
  BOOST_CHECK(r.get_mimm() == ratesvec[2]);
}

BOOST_AUTO_TEST_CASE(test_set_function)
{
  const double rate = -1;
  const rates r;
  BOOST_CHECK_THROW(
  r.set_mimm(rate),
  std::invalid_argument
  );

  BOOST_CHECK_THROW(
  r.set_iclad(rate),
  std::invalid_argument
  );

}

/*BOOST_AUTO_TEST_CASE()
{

}
*/
#pragma GCC diagnostic pop
