#include <fstream>
#include <iostream>
#include <string>
#include <boost/algorithm/string/split.hpp>
#include <boost/test/unit_test.hpp>
#include "kewe_individual.h"
#include "kewe_simulation.h"
#include "kewe_SES.h"
#include "kewe_parameters.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

/*bool is_regular_file(const std::string& filename) noexcept
{
  std::fstream f;
  f.open(filename.c_str(),std::ios::in);
  return f.is_open();
}

///FileToVector reads a file and converts it to a std::vector<std::string>
///From http://www.richelbilderbeek.nl/CppFileToVector.htm
std::vector<std::string> file_to_vector(const std::string& filename)
{
  assert(is_regular_file(filename));
  std::vector<std::string> v;
  std::ifstream in(filename.c_str());
  for (int i=0; !in.eof(); ++i)
  {
    std::string s;
    std::getline(in,s);
    v.push_back(s);
  }
  return v;
}

///From http://www.richelbilderbeek.nl/CppSeperateString.htm
std::vector<std::string> seperate_string(
  const std::string& input,
  const char seperator)
{
  std::vector<std::string> v;
  boost::algorithm::split(v,input,
  std::bind2nd(std::equal_to<char>(),seperator),
  boost::algorithm::token_compress_on);
  return v;
}*/

bool doubles_are_similar_enough(
    const std::vector<double>& x,
    const std::vector<double>& y,
    const double epsilon)
{
  assert(x.size() == y.size());

  for (int i = 2; i < 7; ++i)
    {
      if (std::abs(x[i] - y[i]) > epsilon)
        {
          std::cout << x[i] << " and " << y[i] << " are not similar enough.\n";
          return false;

        }
    }

  return true;


}

/*BOOST_AUTO_TEST_CASE(test_kewe_simulation)
{
  simulation s; //Testing parameters
  s.run();
  BOOST_CHECK(is_regular_file("ltt.csv"));
}*/

BOOST_AUTO_TEST_CASE(test_kewe_can_recreate_golden_output)
{
  const std::string golden_output_filename{"golden_output"};
  recreate_golden_output(golden_output_filename);
  BOOST_CHECK(is_regular_file(golden_output_filename));
}

BOOST_AUTO_TEST_CASE(test_kewe_output_similar)
{
  const std::string golden_output_filename{"golden_output"};
  recreate_golden_output(golden_output_filename);
  kewe_parameters parameters = read_parameters("testparameters");
  simulation s(parameters);
  const kewe_parameters paraCheck = s.get_parameters();
  std::cout << "Nx = " << paraCheck.sim_parameters.Nx << '\n';
  std::cout << "Np = " << paraCheck.sim_parameters.Np << '\n';
  std::cout << "Nq = " << paraCheck.sim_parameters.Nq << '\n';

  s.run();

  const auto output = file_to_vector("defaultresults");
  const auto expected = file_to_vector(golden_output_filename);
  BOOST_CHECK_EQUAL(output.size(), expected.size());
  assert(output.size() > 0);
  assert(expected.size() > 0);

  const auto splitOutput = seperate_string(output[0], ',');
  const auto splitExpected = seperate_string(expected[0], ',');

  std::vector<double> d_output;
  std::vector<double> d_expected;
  std::string::size_type sz;


  for (int i = 0; i < static_cast<int>(splitOutput.size()); ++i)
    {
      d_output.push_back(std::stod(splitOutput[i],&sz));
      d_expected.push_back(std::stod(splitExpected[i],&sz));
    }

  double relativeEpsilon = 1.0;

  BOOST_CHECK(doubles_are_similar_enough(d_output, d_expected, relativeEpsilon));


  std::clog << std::string(40,'*') << "\n\n";
  std::clog << output[0] << '\n' <<  expected[0] << "\n\n";
  std::clog << std::string(40,'*')<< '\n';
}

/*
BOOST_AUTO_TEST_CASE(test_kewe_simulation_no_branching)
{
  simulation s(create_test_parameters_with_no_branching());
  s.run();
  const results r = s.get_results();
  BOOST_CHECK(count_final_n_species_(r) == 1);
}
BOOST_AUTO_TEST_CASE(test_kewe_simulation_branching)
{
  simulation s(create_test_parameters_with_branching());
  s.run();
  const results r = s.get_results();
  BOOST_CHECK(count_final_n_species_(r) > 1);
}
*/

#pragma GCC diagnostic pop
