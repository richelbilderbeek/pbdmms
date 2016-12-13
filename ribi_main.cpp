#include <iostream>
#include "ribi_helper.h"
#include "ribi_menu_dialog.h"

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
