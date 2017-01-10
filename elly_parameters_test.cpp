#include "elly_parameters.h"

#include <sstream>
#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string/split.hpp>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

using namespace elly;

BOOST_AUTO_TEST_CASE(elly_parameters_settings)
{
  const parameters a = create_parameters_set1();
  const parameters b = create_parameters_set1();
  BOOST_CHECK(a.get_ana_rate() == b.get_ana_rate());

}

BOOST_AUTO_TEST_CASE(elly_all_parameters_settings)
{
  const parameters a = create_parameters_set1();
  const parameters b = create_parameters_set1();
  const parameters c = create_parameters_set2();
  BOOST_CHECK(a == a);
  BOOST_CHECK(a == b);
  BOOST_CHECK(a != c);
  BOOST_CHECK(b == a);
  BOOST_CHECK(b == b);
  BOOST_CHECK(b != c);
  BOOST_CHECK(c != a);
  BOOST_CHECK(c != b);
  BOOST_CHECK(c == c);
}

BOOST_AUTO_TEST_CASE(elly_create_parameters_sets)
{
  BOOST_CHECK_NO_THROW(create_parameters_set1());
  BOOST_CHECK_NO_THROW(create_parameters_set2());
  BOOST_CHECK_NO_THROW(create_parameters_set3());
  BOOST_CHECK_NO_THROW(create_profiling_parameters());
}

BOOST_AUTO_TEST_CASE(elly_parameters_operator_stream_out)
{
  const auto p = create_parameters_set1();
  std::stringstream s;
  s << p;
  BOOST_CHECK(!s.str().empty());
}

BOOST_AUTO_TEST_CASE(elly_parameters_operator_streaming_once)
{
  const parameters a = create_parameters_set1();
  parameters b = create_parameters_set2();
  assert(a != b);
  std::stringstream s;
  s << a;
  s >> b;
  BOOST_CHECK_EQUAL(a,b);
}

BOOST_AUTO_TEST_CASE(elly_parameters_operator_streaming_twice)
{
  const parameters a = create_parameters_set2();
  const parameters b = create_parameters_set3();
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

BOOST_AUTO_TEST_CASE(elly_parameters_save_and_load_must_be_symmetrical)
{
  {
    const std::string filename{
      "elly_parameters_save_and_load_must_be_symmetrical_1.txt"};
    const parameters p = create_parameters_set1();
    save(p, filename);
    const parameters q = load_parameters_from_file(filename);
    BOOST_CHECK_EQUAL(p, q);
  }
  {
    const std::string filename{
      "elly_parameters_save_and_load_must_be_symmetrical_2.txt"};
    const parameters p = create_parameters_set2();
    save(p, filename);
    const parameters q = load_parameters_from_file(filename);
    BOOST_CHECK_EQUAL(p, q);
  }
  {
    const std::string filename{
      "elly_parameters_save_and_load_must_be_symmetrical_3.txt"};
    const parameters p = create_parameters_set3();
    save(p, filename);
    const parameters q = load_parameters_from_file(filename);
    BOOST_CHECK_EQUAL(p, q);
  }
  {
    const std::string filename{
      "elly_parameters_save_and_load_must_be_symmetrical_profiling.txt"};
    const parameters p = create_profiling_parameters();
    save(p, filename);
    const parameters q = load_parameters_from_file(filename);
    BOOST_CHECK_EQUAL(p, q);
  }
}

#pragma GCC diagnostic pop
