#include "elly_parameters.h"
#include "elly_event_rates.h"
#include "elly_species.h"
#include "elly_gillespie.h"
#include "elly_events.h"
#include "elly_simulation.h"
#include "elly_experiment.h"
#include "elly_parameters.h"
#include "daic_output.h"
#include "daic_run.h"
#include "daic_helper.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <cstdlib>
#include <exception>

void create_all_parameter_files()
{
  {
    std::ofstream f("parameters_1.txt");
    f << elly::create_parameters_set1() << '\n';
  }
  {
    std::ofstream f("parameters_2.txt");
    f << elly::create_parameters_set2() << '\n';
  }
  {
    std::ofstream f("parameters_3.txt");
    f << elly::create_parameters_set3() << '\n';
  }
  {
    std::ofstream f("parameters_profiling.txt");
    f << elly::create_profiling_parameters() << '\n';
  }
}

/// @param exe_path the path of the executable. This is important, as R needs
///   to have this set explicitly
/// @param filename name of the parameter file
void run_from_file(
  const std::string& exe_path,
  const std::string& filename
)
{
  daic::set_r_working_directory(exe_path);
  const elly::parameters p{
    elly::load_parameters_from_file(filename)
  };

  std::cout << "Running sim with parameters:\n" << p << '\n';

  elly::experiment e(p);
  e.run_sim();

  std::cout << "Result of simulation:\n" << e.get_sim_results() << '\n';

  e.create_daisie_input();

  std::cout
    << "Input for DAISIE, with mainland extinction:\n"
    << e.get_input_ideal() << '\n';

  std::cout
    << "Input for DAISIE, without mainland extinction:\n"
    << e.get_input_reality() << '\n';

  e.run_daisie();

  std::cout
    << "DAISIE estimates, with mainland extinction:\n"
    << e.get_output_ideal() << '\n'
    << "DAISIE estimates, without mainland extinction:\n"
    << e.get_output_reality() << '\n'
  ;
}

///Only run the simulation, not the full experiment with DAISIE
void run_profile()
{
  const elly::parameters p = elly::create_profiling_parameters();
  elly::simulation s(p);
  const double t_end{p.get_crown_age() / 10.0};
  while (s.get_time() < t_end)
  {
    std::clog << ((s.get_time() / t_end) * 100.0) << "%\n";
    s.do_next_event();
  }
}

void show_help() noexcept
{
  std::cout
    << "elly" << '\n'
    << "====" << '\n'
    << '\n'
    << "Usage" << '\n'
    << '\n'
    << " * Run with parameters read from file" << '\n'
    << '\n'
    << "   elly [filename]" << '\n'
    << "   elly elly_parameters_1.txt" << '\n'
    << '\n'
    << '\n'
    << " * Create example parameter files" << '\n'
    << '\n'
    << "   elly --create" << '\n'
    << '\n'
    << " * Do profiling run (without DAISIE)" << '\n'
    << '\n'
    << "   elly --profile" << '\n'
  ;
}

int main(int argc, char* argv[])
{
  try
  {
    const std::string exe_name(argv[0]);
    if (argc != 2)
    {
      show_help();
      return 0;
    }
    const std::string argument(argv[1]);
    if (argument == "--profile")
    {
      run_profile();
      return 0;
    }
    if (argument == "--create")
    {
      create_all_parameter_files();
      return 0;
    }
    //Argument is the filename of the parameter
    run_from_file(daic::get_path(exe_name), argument);
  }
  catch (std::exception& e)
  {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }
  catch (...)
  {
    std::cerr << "Error: Unknown\n";
    return 1;
  }
}

