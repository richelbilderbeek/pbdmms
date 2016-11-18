#include "jobo_simulation_test.h"
#include "jobo_individual_test.h"
#include "jobo_output.h"
#include "jobo_output_test.h"
#include <cassert>
#include <string>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <cctype>
#include "jobo_individual.h"
#include "jobo_parameters.h"
#include "jobo_simulation.h"
#include <boost/test/unit_test.hpp>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

using namespace jobo;

BOOST_AUTO_TEST_CASE(test_jobo_get_m_ltt_good)
{
  const int time (10);
  const double mutation_rate (0.5);
  std::mt19937 rng_engine(42);
  std::vector<individual> individuals(10, individual("aBCdEfGhIj"));
  //Create vector with number of good species per generation, starting with good species 1
  vector<int> m_ltt_good = get_m_ltt_good(
      time,mutation_rate, rng_engine,individuals
      );
  BOOST_CHECK (m_ltt_good.size() >= 1);
  BOOST_CHECK (m_ltt_good.size() == 10);
}

#pragma GCC diagnostic pop
