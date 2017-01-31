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

BOOST_AUTO_TEST_CASE(pbd_ltt_get_n_interpolates_correctly)
{
  //
  // n (number of lineages)
  //
  // 4 +           +---+
  // 3 |       +---+
  // 2 |   +---+
  // 1 +---+
  // 0 +---+---+---+---+
  //   0   1   2   3   4  t (time units)
  //
  ltt my_ltt;
  my_ltt.add_timepoint(0.0, 1);
  my_ltt.add_timepoint(1.0, 2);
  my_ltt.add_timepoint(2.0, 3);
  my_ltt.add_timepoint(3.0, 4);
  my_ltt.add_timepoint(4.0, 4);

  //Reading
  BOOST_CHECK_EQUAL(my_ltt.get_n(0.0), 1);
  BOOST_CHECK_EQUAL(my_ltt.get_n(1.0), 2);
  BOOST_CHECK_EQUAL(my_ltt.get_n(2.0), 3);
  BOOST_CHECK_EQUAL(my_ltt.get_n(3.0), 4);
  BOOST_CHECK_EQUAL(my_ltt.get_n(4.0), 4);
  //Interpolate
  BOOST_CHECK_EQUAL(my_ltt.get_n(0.0 + 0.5), 1);
  BOOST_CHECK_EQUAL(my_ltt.get_n(1.0 + 0.5), 2);
  BOOST_CHECK_EQUAL(my_ltt.get_n(2.0 + 0.5), 3);
  BOOST_CHECK_EQUAL(my_ltt.get_n(3.0 + 0.5), 4);
  //Extrapolate
  BOOST_CHECK_EQUAL(my_ltt.get_n(9.0), 4);
}
#pragma GCC diagnostic pop






