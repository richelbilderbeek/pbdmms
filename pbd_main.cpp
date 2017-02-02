#include "pbd_menu.h"
#include "pbd.h"
#include "pbd_helper.h"
#include "pbd_parameters.h"
#include "pbd_helper.h"
#include "pbd_l_table.h"
#include "pbd_l_table_row.h"
#include "pbd_ltt.h"
#include "pbd_nltt.h"
#include <cassert>
#include <exception>
#include <iterator>
#include <iostream>
#include <fstream>

using namespace pbd;

///Does not return the filename
std::vector<std::string> get_args(int argc, char * argv[])
{
  std::vector<std::string> v;
  v.reserve(argc - 1);
  for (int i=1; i!=argc; ++i) //Skip the filename
  {
    v.push_back(std::string(argv[i]));
  }
  return v;
}

int main(int argc, char * argv[])
{
  try
  {
    const std::vector<std::string> args = get_args(argc, argv);
    menu m(args);
    m.run();
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


