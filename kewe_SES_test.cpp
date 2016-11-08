#include <fstream>
#include <iostream>
#include <boost/test/unit_test.hpp>
#include "kewe_individual.h"
#include "kewe_simulation.h"
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

  simulation s;
  s.run();
  const auto results = file_to_vector("defaultresults");
  const auto expected = file_to_vector(golden_output_filename);
  BOOST_CHECK_EQUAL(results.size(), expected.size());
  assert(results.size() > 0);
  assert(expected.size() > 0);
  BOOST_CHECK_EQUAL(results[0], expected[0]);
  std::clog << std::string(40,'*') << '\n';
  std::clog << results[0] << '\n' <<  expected[0] << '\n';
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
