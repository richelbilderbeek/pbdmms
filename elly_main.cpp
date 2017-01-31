#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "elly_menu.h"

int main(int argc, char* argv[])
{
  try
  {
    std::vector<std::string> args;
    for (int i=0; i!=argc; ++i)
    {
      args.push_back(std::string(argv[i]));
    }
    elly::menu m(args);
    m.execute();

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

