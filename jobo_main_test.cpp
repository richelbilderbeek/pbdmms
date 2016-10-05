//#include "distancer_simulation.h"
#include <cassert>
#include <exception>
#include <iostream>
#include <fstream>
#include "jobo_helper.h"

int main() {
  try
  {
    hello_jobo();
    assert(add(40,2) == 42);
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
