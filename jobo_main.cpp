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
#include "jobo_output.h"

using namespace jobo;

int main() {
  try
  {
    //const vector<individual> individuals{10, individual("aBCdEfG")};
    const parameters a(1,38,0.5,10,6);

    jkr::do_experiment<
      jobo::parameters,
      jobo::simulation,
      jobo::results
    >(a);
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
