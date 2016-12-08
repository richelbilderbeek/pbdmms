#include "jkr_experiment.h"
#include "jobo_jkr_adapters.h"
#include "jobo_parameters.h"
#include "jobo_simulation.h"
#include "jobo_results.h"
#include <exception>
#include <iostream>
#include <fstream>
#include "jobo_parameters.h"
#include "jobo_simulation.h"

using namespace jobo;

int main(int argc, char * argv[]) {
  for (int i=0; i!=argc; ++i)
  {
    std::cout << i << ": " << argv[i] << '\n';
  }

  try
  {
    /*
    // Use parameters from parameter file:
    jobo::parameters a;
    {
       std::ofstream f("jobo_parameters.txt");
       f << a;
    }
    */
    // Or direct input:
    if (argc == 1)
    {
      const parameters a(2,38,0.5,10,6,0.05);

      jkr::do_experiment<
        jobo::parameters,
        jobo::simulation,
        jobo::results
      >(a);
    }
    else if (std::string(argv[1]) == "--profile")
    {
      const parameters a(2000,38,0.5,10,6,0.05);
      jkr::do_experiment<
        jobo::parameters,
         jobo::simulation,
          jobo::results
        >(a);
    }
    else
    {
      const std::string filename = argv[1];
      const parameters a = load_parameters(filename);
      jkr::do_experiment<
        jobo::parameters,
        jobo::simulation,
        jobo::results
       >(a);
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
