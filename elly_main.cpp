#include <iostream>
#include "elly_parameters.h"

int main()
{
  try
  {
    using namespace elly;

        elly::parameters create_parameter_set1();
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
