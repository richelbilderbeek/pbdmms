#include "jkr_experiment.h"
#include "jobo_jkr_adapters.h"
#include "jobo_parameters.h"
#include "jobo_simulation.h"
#include "jobo_results.h"
#include <exception>
#include <iostream>
#include <fstream>
#include "jobo_parameters.h"

using namespace jobo;

int main(int argc, char * argv[])
{
  std::clog << "Version 2017-01-27-12:57\n";
  try
  {
    if (argc == 1)
    { //If program has been called without arguments, do a default run
      const parameters a = create_test_parameters_1();
      jobo::simulation s(a);
       s.run();
    }
    else if (std::string(argv[1]) == "--profile")
    { //If program has been called like './jobo --profile', do a profile run
      const parameters a = create_profiling_parameters();
      jkr::do_experiment<
        jobo::parameters,
        jobo::simulation,
        jobo::results
        >(a);
    }
    else
    {
      //Assume the second argument, './jobo something.txt' is the filename of a parameters file.
      //Use that one to load parameters and start the sim
      const std::string filename = argv[1];
      const parameters p = load_parameters(filename);
      std::clog << "Parameters loaded: " << p << '\n';
      simulation s(p);
      s.run();
      const auto& r = s.get_results();
      save_ltt_plot_viables(r, p.get_ltt_plot_filename_vi());
      save_ltt_plot_inviables(r, p.get_ltt_plot_filename_in());
      save_nltt_plot_viables(r, p.get_nltt_plot_filename_v());
      save_nltt_plot_inviables(r, p.get_nltt_plot_filename_i());
      //jkr::do_experiment<
      //  jobo::parameters,
      //  jobo::simulation,
      //  jobo::results
      // >(a);
    }
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
