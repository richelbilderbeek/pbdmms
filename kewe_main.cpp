#include <chrono>
#include <iostream>
#include <QFile>
#include <string>
#include <fstream>
#include <random>
#include "kewe_simulation.h"
#include "jkr_experiment.h"
#include "kewe_results.h"
#include "kewe_ses.h"
#include "kewe_helper.h"
#include "kewe_gausser.h"
#include "kewe_jkr_adapters.h"
#include "kewe_parameters.h"

using namespace kewe;

void benchmark_gausser()
{
  const double sd{1.0};
  const double min_x{-4.0};
  const double max_x{4.0};
  const double dx{8.0 / 100000000.0};
  double sum_gauss{0.0};
  double sum_gausser{0.0};
  int n_millis_gauss{0};
  int n_millis_gausser{0};

  namespace cr = std::chrono;

  // you can replace this with steady_clock or system_clock
  typedef cr::high_resolution_clock my_clock;

  //gauss
  {
    const auto start_time = my_clock::now();
    {
      for (double x{min_x}; x<max_x; x+=dx)
      {
        sum_gauss += gauss(x, sd);
      }
    }
    const auto end_time = my_clock::now();
    const auto diff = end_time - start_time;
    n_millis_gauss = cr::duration_cast<cr::milliseconds>(diff).count();
  }
  //gausser
  {
    const auto start_time = my_clock::now();
    {
      const gausser g(sd);
      for (double x{min_x}; x<max_x; x+=dx)
      {
        sum_gausser += g(x);
      }
    }
    const auto end_time = my_clock::now();
    const auto diff = end_time - start_time;
    n_millis_gausser = cr::duration_cast<cr::milliseconds>(diff).count();
  }
  std::clog << "sum_gauss: " << sum_gauss << '\n';
  std::clog << "sum_gausser: " << sum_gausser << '\n';
  std::clog << "n_millis_gauss: " << n_millis_gauss << '\n';
  std::clog << "n_millis_gausser: " << n_millis_gausser << '\n';
}

int main(int argc, char* argv[])
{
  if (argc == 2 && std::string(argv[1]) == std::string("--gausser"))
  {
    benchmark_gausser();
    return 0;
  }
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
