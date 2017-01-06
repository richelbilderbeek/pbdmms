#include "elly_parameters.h"
#include "elly_event_rates.h"
#include "elly_species.h"
#include "elly_gillespie.h"
#include "elly_events.h"
#include "elly_simulation.h"
#include "elly_experiment.h"
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


int main(int argc, char* argv[])
{
  try
  {
    //using namespace elly;
    if (argc == 2 && std::string(argv[1]) == std::string("--profile"))
    {
      //Only run the simulation
      const elly::parameters p = elly::create_profiling_parameters();
      elly::simulation s(p);
      s.run();
      return 0;
    }
    daic::set_r_working_directory(daic::get_path(argv[0]));
    const elly::parameters p = elly::create_parameters_set1();
    const elly::experiment e(p);
    std::cout
      << "DAISIE estimates, with mainland extinction:\n"
      << e.get_do_with_main_ext() << '\n'
      << "DAISIE estimates, without mainland extinction:\n"
      << e.get_do_without_main_ext() << '\n'
    ;
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

