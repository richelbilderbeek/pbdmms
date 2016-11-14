#include "ribi_menu_dialog.h"
#include "ribi_helper.h"

int my_main(int argc, char* argv[])
{
    ribi::menu_dialog d;
    const std::vector<std::string> args{
      ribi::convert_arguments(argc,argv)
    };
    return d.execute(args);
}

int main(int argc, char* argv[])
{
  return ribi::exception_catching_main(
    my_main,
    argc, argv
  );
}
