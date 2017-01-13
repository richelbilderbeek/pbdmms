#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "elly_analyse.h"

///Analyses the log files created on Peregrine
int main(int argc, char* argv[])
{
  try
  {
    std::vector<std::string> args;
    for (int i=1; i!=argc; ++i) //Skip the exe itself
    {
      args.push_back(std::string(argv[i]));
    }
    const std::vector<std::string> lines = elly::analyse(args);
    for (const auto& line: lines)
    {
      std::cout << line << '\n';
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

