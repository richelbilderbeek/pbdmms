#ifndef ELLY_MENU_H
#define ELLY_MENU_H

#include <string>
#include <vector>

namespace elly {

///The CLI menu
class menu
{
public:
  explicit menu(const std::vector<std::string>& args);
};


///Save all testing parameter files
void create_all_parameter_files() noexcept;

/// @param exe_path the path of the executable. This is important, as R needs
///   to have this set explicitly
/// @param filename name of the parameter file
void run_from_file(
  const std::string& exe_path,
  const std::string& filename
);

///Do a profiling run
///Only run the simulation, not the full experiment with DAISIE
void run_profile();

///Show the help
void show_help() noexcept;

} //~namespace elly

#endif // ELLY_MENU_H
