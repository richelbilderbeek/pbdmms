//#include "distancer_simulation.h"
#include <exception>
#include <iostream>
#include <fstream>
//#include "distancer_helper.h"
//#include "is_regular_file.h"
//#include "convert_dot_to_svg.h"
//#include "convert_svg_to_png.h"

int main() {
  try
  {
    std::cout<<"Hello world";
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
