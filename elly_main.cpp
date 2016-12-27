#include <cassert>
#include <iostream>
#include "elly_parameters.h"
#include "elly_event_rates.h"
#include "elly_species.h"
#include "elly_gillespie.h"
#include "elly_events.h"
#include "elly_simulation.h"
#include <vector>
#include <random>
#include <chrono>
#include <cstdlib>
#include <exception>


int main(int argc, char* argv[])
{
  try
  {
    using namespace elly;
    if (argc == 2 && std::string(argv[1]) == std::string("--profile"))
    {
      const parameters p = create_profiling_parameters();
      simulation s(p);
      s.run();
      return 0;
    }
    const parameters p = create_parameters_set1();
    simulation s(p);
    s.run();
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
