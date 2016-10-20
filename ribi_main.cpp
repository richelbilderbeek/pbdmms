#include "ribi_menu_dialog.h"

#include "ribi_simulation.h"
#include <exception>
#include <iostream>
#include <fstream>
#include "ribi_helper.h"
#include "is_regular_file.h"
#include "convert_dot_to_svg.h"
#include "convert_svg_to_png.h"

int main(int argc, char* argv[])
{
  try
  {
    ribi::menu_dialog d;
    const std::vector<std::string> args{
      ribi::convert_arguments(argc,argv)
    };
    return d.execute(args);
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
