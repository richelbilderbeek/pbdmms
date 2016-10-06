//#include "distancer_simulation.h"
#include <exception>
#include <iostream>
#include <fstream>
#include "jobo_helper.h"

int main() {
  try
  {
    hello_jobo();
    std::cout << add(40,2) << '\n';
    std::cout << divide(1,2) << '\n';
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
