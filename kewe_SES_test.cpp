#include <fstream>
#include <iostream>
#include <boost/test/unit_test.hpp>
#include "kewe_individual.h"
#include "kewe_SES.h"
// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

bool is_regular_file(const std::string& filename) noexcept
{
  std::fstream f;
  f.open(filename.c_str(),std::ios::in);
  return f.is_open();
}

BOOST_AUTO_TEST_CASE(test_kewe_simulation)
{
  const std::string filename("test_kewe_simulation.csv");
  create_test_parameter_file(filename);
  readparameters(filename.c_str());
  initialize();

  std::vector<std::vector<double>> histX;
  std::vector<std::vector<double>> histP;
  std::vector<std::vector<double>> histQ;

  iterate(histX, histP, histQ);

  outputLTT(histX, histP, histQ);
  //Check the output
  BOOST_CHECK(is_regular_file("ltt.csv"));
}

#pragma GCC diagnostic pop








