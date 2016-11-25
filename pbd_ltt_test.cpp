#include "pbd_ltt.h"
#include <fstream>
#include <iostream>
#include <boost/test/unit_test.hpp>

#include "pbd_helper.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

using namespace pbd;

BOOST_AUTO_TEST_CASE(pbd_ltt_comparisons)
{
  const ltt p = create_test_ltt();
  const ltt q = create_test_ltt();
  ltt r = create_test_ltt();
  r.add_timepoint(23,7);
  BOOST_CHECK_EQUAL(p, p);
  BOOST_CHECK_EQUAL(p, q);
  BOOST_CHECK_NE(p, r);
  BOOST_CHECK_EQUAL(q, p);
  BOOST_CHECK_EQUAL(q, q);
  BOOST_CHECK_NE(q, r);
  BOOST_CHECK_NE(r, p);
  BOOST_CHECK_NE(r, q);
  BOOST_CHECK_EQUAL(r, r);
}

BOOST_AUTO_TEST_CASE(pbd_ltt_save_and_load_should_be_symmetrical)
{
  const std::string filename("pbd_ltt_save_and_load_should_be_symmetrical");

  if (is_regular_file(filename)) { delete_file(filename); }
  assert(!is_regular_file(filename));

  const ltt p = create_test_ltt();
  {
    std::ofstream f(filename);
    f << p;
  }
  const ltt q = load_ltt_from_csv(filename);

  BOOST_CHECK_EQUAL(p, q);

  if (is_regular_file(filename)) { delete_file(filename); }
  assert(!is_regular_file(filename));
}

#pragma GCC diagnostic pop






