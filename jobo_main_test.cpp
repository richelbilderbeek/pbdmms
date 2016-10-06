//#include "distancer_simulation.h"
#include <cassert>
#include <exception>
#include <iostream>
#include <fstream>
#include "jobo_helper.h"

int main() {
  int n_fails{0};
  try
  {
    hello_jobo();
    if (add(40,2) != 42) ++n_fails;
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
  return n_fails;
}
