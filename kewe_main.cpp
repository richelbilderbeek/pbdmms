#include <iostream>
#include <QFile>
#include <string>
#include <fstream>
#include <random>
#include "kewe_simulation.h"
#include "jkr_experiment.h"
#include "kewe_results.h"
#include "kewe_ses.h"
#include "kewe_jkr_adapters.h"
#include "kewe_parameters.h"

using namespace kewe;

int main(int argc, char* argv[])
{
  if (argc == 2 && std::string(argv[1]) == std::string("--profile"))
  {
    parameters p = create_profiling_parameters();
    jkr::do_experiment<kewe::parameters, kewe::simulation, kewe::results>(p);
    return 0;
  }
  if (argc == 2 && std::string(argv[1]) == std::string("--random"))
  {
    parameters p = create_random_run_parameters();
    p.m_output_parameters.outputfreq = 0;
    std::clog << p << '\n';
    simulation s(p);
    s.run();
    std::cout << "Branching on ecotype: " << has_bimodal_eco_types(s) << '\n';
    std::cout << "Branching on mating: " << has_branching_mating(s) << '\n';
    std::cout << "Sympatric speciation: " << has_sympatric_speciation(s) << '\n';
    return 0;
  }
  #ifdef FIX_ISSUE_131
  //====FIX_ISSUE_131====
  QFile f(":/kewe/kewe_testparameters");
  f.copy("testparameters");
  kewe::parameters parameters = kewe::read_parameters("testparameters");
  kewe::simulation s(parameters);
  s.run();
  jkr::do_experiment<kewe::parameters, kewe::simulation, kewe::results>(parameters);
  #endif

  return 0;
}
