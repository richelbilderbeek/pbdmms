#include "jobo_output.h"
#include <cassert>
#include <string>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <cctype>
#include "jobo_individual.h"
#include "jobo_parameters.h"
#include "jobo_simulation.h"
#include "jobo_jkr_adapters.h"
#include <boost/test/unit_test.hpp>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

using namespace jobo;

BOOST_AUTO_TEST_CASE(test_jobo_run_simulation)
{
  // Give all parameters to use in run_simulation function
  //std::vector<individual> individuals(10, individual("aBCdEfGhIj"));
  //const parameters d(10,42,0.5,10);
  // Create vector with number of good species per generation
  /*
  vector<int> m_ltt= run_simulation(d);
  BOOST_CHECK (m_ltt.size() >= 1);
  BOOST_CHECK (m_ltt.size() == 10);
  BOOST_CHECK (m_ltt[0] == 3);
  BOOST_CHECK (m_ltt[1] == 2);
  BOOST_CHECK (m_ltt[8] == 1);
  */
}

BOOST_AUTO_TEST_CASE(test_jobo_run_simulation_2)
{
  /*
  // Give all parameters to use in run_simulation function with genotype of 44 loci
  //std::vector<individual> individuals(10, individual("aBCdEfGhIjKlMnoPQrStUvwXyZAbcDeFgHIjkLMnOpqR"));
  const parameters d(10,42,0.5,20);
  // Create vector with number of good species per generation
  vector<int> m_ltt= run_simulation(d);
  BOOST_CHECK (m_ltt.size() >= 1);
  BOOST_CHECK (m_ltt.size() == 20);
  BOOST_CHECK (m_ltt[0] == 1);
  BOOST_CHECK (m_ltt[1] == 1);
  BOOST_CHECK (m_ltt[8] == 1);
  */
}

BOOST_AUTO_TEST_CASE(test_jobo_create_ltt_plot_filename)
{

  const parameters d(10,42,0.5,10,6);
  std::string filename = get_ltt_plot_filename(d);
  std::string e ("jobo abcdef,10,6,0.5,10,42");
  BOOST_CHECK (filename == e);
}

#pragma GCC diagnostic pop
