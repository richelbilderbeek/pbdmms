#ifndef RIBI_MENU_DIALOG_H
#define RIBI_MENU_DIALOG_H

#include <string>
#include <vector>

#include "ribi_parameters.h"

namespace ribi {

///Takes care of the CLI user interaction
class menu_dialog
{
public:

  ///Run a simulation from the command-line arguments
  ///Throws anything that the process will throw
  int execute(const std::vector<std::string>& args);

private:
  void run(const parameters& p);

  ///Run a demo run
  void run_demo();

  /// @param parameters_filename filename of the parameters file
  void run_from_file(const std::string& parameters_filename);

  void show_help() noexcept;
};

} //namespace ribi

#endif // RIBI_MENU_DIALOG_H
