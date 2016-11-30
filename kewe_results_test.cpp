#include <fstream>
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <random>
#include "kewe_parameters.h"
#include "kewe_results.h"
#include <stdexcept>
#include <cassert>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

BOOST_AUTO_TEST_CASE(kewe_results_test_count_num_border)
{
 int numOfBorders{0};
 count_num_border(0.06, 0.06, 0.06, numOfBorders);
 BOOST_CHECK_EQUAL(numOfBorders, 0);
 count_num_border(0.06, 0.06, 0.04, numOfBorders);
 BOOST_CHECK_EQUAL(numOfBorders, 0);
 count_num_border(0.06, 0.04, 0.06, numOfBorders);
 BOOST_CHECK_EQUAL(numOfBorders, 0);
 count_num_border(0.04, 0.06, 0.06, numOfBorders);
 BOOST_CHECK_EQUAL(numOfBorders, 0);
 count_num_border(0.06, 0.04, 0.04, numOfBorders);
 BOOST_CHECK_EQUAL(numOfBorders, 1);
 count_num_border(0.04, 0.06, 0.04, numOfBorders);
 BOOST_CHECK_EQUAL(numOfBorders, 1);
 count_num_border(0.04, 0.04, 0.06, numOfBorders);
 BOOST_CHECK_EQUAL(numOfBorders, 2);
}

BOOST_AUTO_TEST_CASE(kewe_results_should_throw_if_empty_histogram)
{ const std::vector<double> histogram;
  BOOST_CHECK_THROW(int x = countBorders(histogram), std::invalid_argument);

  std::vector<double> v (3, 1.0);
  std::vector<std::vector<double>> w;
  w.push_back(v);

  std::vector<std::vector<double>> empty;

  BOOST_CHECK_THROW(countLineagesForGen(1, w, w, empty), std::invalid_argument);
  BOOST_CHECK_THROW(countLineagesForGen(1, w, empty, w), std::invalid_argument);
  BOOST_CHECK_THROW(countLineagesForGen(1, empty, w, w), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(kewe_results_should_throw_with_negative_time)
{
  std::vector<double> v (3, 1.0);
  std::vector<std::vector<double>> w;
  w.push_back(v);
  BOOST_CHECK_THROW(countLineagesForGen(-1, w, w, w), std::invalid_argument);

}


BOOST_AUTO_TEST_CASE(kewe_outputLTT_should_throw_if_empty_histogram)
{
  std::vector<double> v (3, 1.0);
  std::vector<std::vector<double>> w;
  w.push_back(v);

  std::vector<std::vector<double>> empty;
  kewe_parameters parameters;
  BOOST_CHECK_THROW(outputLTT(w, w, empty, parameters), std::invalid_argument);
  BOOST_CHECK_THROW(outputLTT(w, empty, w, parameters), std::invalid_argument);
  BOOST_CHECK_THROW(outputLTT(empty, w, w, parameters), std::invalid_argument);

}

BOOST_AUTO_TEST_CASE(kewe_test_)

#pragma GCC diagnostic pop



