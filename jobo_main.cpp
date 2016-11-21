#include <exception>
#include <iostream>
#include <fstream>
#include "jobo_parameters.h"
#include "jobo_simulation.h"
#include "jobo_output.h"

int main() {
  try
  {
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
