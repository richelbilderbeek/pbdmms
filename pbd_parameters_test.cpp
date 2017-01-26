#include "pbd_parameters.h"

#include <fstream>
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <sstream>

#include "pbd_helper.h"
#include "pbd_ltt.h"
#include "pbd_nltt.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

#include <boost/algorithm/string/split.hpp>

using namespace pbd;

BOOST_AUTO_TEST_CASE(pbd_parameters_operator_stream_out)
{
  const auto p = create_parameters_set1();
  std::stringstream s;
  s << p;
  BOOST_CHECK(!s.str().empty());
}

BOOST_AUTO_TEST_CASE(pbd_parameters_operator_streaming_once)
{
  parameters a = create_parameters_set1();
  parameters b = create_parameters_set2();
  assert(a != b);
  std::stringstream s;
  s << a;
  s >> b;
  assert(b == create_parameters_set1());
  assert(b != create_parameters_set2());
}

BOOST_AUTO_TEST_CASE(pbd_parameters_operator_streaming_twice)
{
  parameters a = create_parameters_set2();
  parameters b = create_parameters_set3();
  parameters c = create_parameters_set1();
  parameters d = create_parameters_set1();
  assert(a != b);
  assert(b != d);
  std::stringstream s;
  s << a << ' ' << b;
  s >> c >> d;
  BOOST_CHECK_EQUAL(a,c);
  BOOST_CHECK_EQUAL(b,d);
}

BOOST_AUTO_TEST_CASE(pbd_parameters_save_and_load_must_be_symmetrical)
{
  {
    const std::string filename{
      "pbd_parameters_save_and_load_must_be_symmetrical_1.txt"};
    const parameters p = create_parameters_set1();
    save(p, filename);
    const parameters q = load_parameters_from_file(filename);
    BOOST_CHECK_EQUAL(p, q);
  }
  {
    const std::string filename{
      "pbd_parameters_save_and_load_must_be_symmetrical_2.txt"};
    const parameters p = create_parameters_set2();
    save(p, filename);
    const parameters q = load_parameters_from_file(filename);
    BOOST_CHECK_EQUAL(p, q);
  }
  {
    const std::string filename{
      "pbd_parameters_save_and_load_must_be_symmetrical_3.txt"};
    const parameters p = create_parameters_set3();
    save(p, filename);
    const parameters q = load_parameters_from_file(filename);
    BOOST_CHECK_EQUAL(p, q);
  }
}

#pragma GCC diagnostic pop
