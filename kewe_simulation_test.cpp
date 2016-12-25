#include <iostream>
#include <boost/algorithm/string/split.hpp>

#include "kewe_individual.h"
#include "kewe_simulation.h"
#include "kewe_ses.h"
#include "kewe_parameters.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace kewe;

BOOST_AUTO_TEST_CASE(kewe_simulation_should_always_work)
{
  /*
  parameters p = create_random_run_parameters();
  p.m_sim_parameters.popsize = 10;
  p.m_sim_parameters.set_end_time(10);
  p.m_output_parameters.outputfreq = p.m_sim_parameters.get_end_time() - 2;
  std::cout << "*** NEW RUN ***" << '\n';
  std::cout << p << '\n';
  simulation s(p);
  s.run();
  std::cout << "Branching on ecotype: " << has_bimodal_eco_types(s) << '\n';
  std::cout << "Branching on mating: " << has_branching_mating(s) << '\n';
  std::cout << "Sympatric speciation: " << has_sympatric_speciation(s) << '\n';
  */
}
#pragma GCC diagnostic pop
