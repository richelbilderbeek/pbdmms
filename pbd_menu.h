#ifndef PBD_MENU_H
#define PBD_MENU_H

#include <string>
#include <vector>

namespace pbd {

struct parameters;

///Command-line interface
class menu
{
public:
  ///@param args_without_filename all argv arguments except argv[0]
  menu(const std::vector<std::string>& args);

  ///Throws an exception if the arguments could not be parsed
  void run();

private:
  const std::vector<std::string> m_args;
};

///Create an example parameter file
///Throws an exception if that file is already present
void create();

///Execute the menu with multiple arguments
void run_from_args(const std::vector<std::string>& args);

///Execute the menu when the user supplied one argument
void run_one_arg(const std::string& arg);

///Do a profile run
void run_profile();

///Run a simulation from a parameter filename
void run_from_file(const std::string& filename);

///Run the simulation
void run_sim(const parameters& p);

///Show the help of pbd
void show_help() noexcept;

///Shows the version and/or compilation time and date
void show_version() noexcept;

} //~namespace pbd

#endif // PBD_MENU_H
