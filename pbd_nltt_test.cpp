#include "pbd_nltt.h"

#include <fstream>
#include <iostream>
#include <boost/test/unit_test.hpp>

#include "pbd_helper.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

using namespace pbd;

BOOST_AUTO_TEST_CASE(pbd_nltt_comparisons)
{
  const nltt p = create_test_nltt();
  const nltt q = create_test_nltt();
  const nltt r;
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

  const nltt p = create_test_nltt();
  {
    std::ofstream f(filename);
    f << p;
  }
  const nltt q = load_nltt_from_csv(filename);

  BOOST_CHECK(p == q);

  if (is_regular_file(filename)) { delete_file(filename); }
  assert(!is_regular_file(filename));
}

#pragma GCC diagnostic pop





