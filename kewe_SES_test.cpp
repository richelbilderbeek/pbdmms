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
  simulation s; //Testing parameters
  s.run();
  BOOST_CHECK(is_regular_file("ltt.csv"));
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
