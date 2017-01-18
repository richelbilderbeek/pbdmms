#include "pbd_nltt.h"

#include <fstream>
#include <iostream>
#include <boost/test/unit_test.hpp>

#include "pbd_helper.h"
#include "pbd_ltt.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

using namespace pbd;

BOOST_AUTO_TEST_CASE(pbd_nltt_comparisons)
{
  const nltt p = create_test_nltt_1();
  const nltt q = create_test_nltt_1();
  const nltt r = create_test_nltt_2();
  BOOST_CHECK(p == p);
  BOOST_CHECK(p == q);
  BOOST_CHECK(p != r);
  BOOST_CHECK(q == p);
  BOOST_CHECK(q == q);
  BOOST_CHECK(q != r);
  BOOST_CHECK(r != p);
  BOOST_CHECK(r != q);
  BOOST_CHECK(r == r);
}

BOOST_AUTO_TEST_CASE(pbd_nltt_save_and_load_should_be_symmetrical)
{
  const std::string filename("pbd_nltt_save_and_load_should_be_symmetrical");

  if (is_regular_file(filename)) { delete_file(filename); }
  assert(!is_regular_file(filename));

  const nltt p = create_test_nltt_1();
  {
    std::ofstream f(filename);
    f << p;
  }
  const nltt q = load_nltt_from_csv(filename);

  BOOST_CHECK(p == q);

  if (is_regular_file(filename)) { delete_file(filename); }
  assert(!is_regular_file(filename));
}

BOOST_AUTO_TEST_CASE(pbd_nltt_save_all_examples_to_csv)
{
  const std::vector<nltt> nltts = {
    create_test_nltt_1(),
    create_test_nltt_2(),
    create_test_nltt_3()
  };
  const int n = nltts.size();
  for (int i=0; i!=n; ++i)
  {
    std::ofstream f("pbd_test_nltt_" + std::to_string(i + 1) + ".csv");
    f << nltts[i];
  }
}

BOOST_AUTO_TEST_CASE(pbd_nltt_get_nltt_statistic)
{
  const nltt a = create_test_nltt_1();
  const nltt b = create_test_nltt_2();
  const nltt c = create_test_nltt_3();


  BOOST_CHECK_CLOSE(calc_nltt_statistic(a,a),0.0 , 0.0001);
  BOOST_CHECK_CLOSE(calc_nltt_statistic(b,c),0.25, 0.0001);
}

BOOST_AUTO_TEST_CASE(pbd_convert_ltt_to_nltt)
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
  // should become
  //
  // n (normalized number of lineages)
  //
  // 1.00  +           +---+
  // 0.75  |       +---+
  // 0.50  |   +---+
  // 0.25  +---+
  // 0.00  +---+---+---+---+
  //       0      0.5      1  t (normalized time units)
  //
  ltt a;
  a.add_timepoint(0.0, 1);
  a.add_timepoint(1.0, 2);
  a.add_timepoint(2.0, 3);
  a.add_timepoint(3.0, 4);
  a.add_timepoint(4.0, 4);

  nltt expected;
  expected.push_back(std::make_pair(0.00,0.25));
  expected.push_back(std::make_pair(0.25,0.50));
  expected.push_back(std::make_pair(0.50,0.75));
  expected.push_back(std::make_pair(0.75,1.00));
  expected.push_back(std::make_pair(1.00,1.00));

  const nltt measured = convert_to_nltt(a);
  BOOST_CHECK(measured  == expected);




}



#pragma GCC diagnostic pop






