#include <fstream>
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string/split.hpp>
#include <random>
#include <string>
#include "kewe_parameters.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

using namespace kewe;

BOOST_AUTO_TEST_CASE(kewe_parameters_throw_when_too_many_multiple_parameters1)
{
  #ifdef WHEN_PARAMETER_CLASS_IS_CORRECT
  create_test_parameter_file1("testfile");
  BOOST_CHECK_THROW(read_parameters("testfile"), std::invalid_argument);
  #endif // WHEN_PARAMETER_CLASS_IS_CORRECT
}

BOOST_AUTO_TEST_CASE(kewe_parameters_throw_when_too_many_multiple_parameters2)
{
  #ifdef WHEN_PARAMETER_CLASS_IS_CORRECT
  create_test_parameter_file2("testfile");
  BOOST_CHECK_THROW(read_parameters("testfile"), std::invalid_argument);
  #endif // WHEN_PARAMETER_CLASS_IS_CORRECT
}

BOOST_AUTO_TEST_CASE(kewe_parameters_throw_when_too_many_multiple_parameters3)
{
  create_test_parameter_file3("testfile");
  BOOST_CHECK_THROW(read_parameters("testfile"), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(kewe_parameters_can_read_haploid_and_diploid_0_and_1)
{
  create_test_parameter_file4("testfile");
  read_parameters("testfile");
  create_test_parameter_file5("testfile");
  read_parameters("testfile");
  create_test_parameter_file6("testfile");
  read_parameters("testfile");
}

#pragma GCC diagnostic pop



