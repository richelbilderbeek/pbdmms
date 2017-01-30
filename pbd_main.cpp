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
#include <iostream>
#include <fstream>

using namespace pbd;

void create() noexcept
{
  const std::string filename{"pbd.txt"};
  if (!is_regular_file(filename))
  {
    std::ofstream f(filename);
    f << create_parameters_set1();
  }
}

void show_file(const std::string& filename)
{
  std::cout << "Showing the file:\n";
  for (const std::string& line: file_to_vector(filename))
  {
    std::cout << line << '\n';
  }
  std::cout << "Done showing the file.\n";
}

void show_help() noexcept
{
  std::cout
    << "pbd" << '\n'
    << "---" << '\n'
    << "" << '\n'
    << "Run a PBD simulation" << '\n'
    << "" << '\n'
    << "Use: " << '\n'
    << "" << '\n'
    << "  Run a PBD simulation,"<< '\n'
    << "    with parameter filename 'pbd.txt':" << '\n'
    << "" << '\n'
    << "    ./pbd pbd.txt" << '\n'
    << "" << '\n'
    << "  Create a PBD parameter input file."
    << "  It will have filename 'pbd.txt'." << '\n'
    << "  Will do nothing if this file already exist." << '\n'
    << "" << '\n'
    << "    ./pbd --create" << '\n'
    << "" << '\n'
    << "  Do a profiling run:" << '\n'
    << "" << '\n'
    << "    ./pbd --profile" << '\n'
    << "" << '\n'
  ;
}

int main(int argc, char * argv[])
{
  try
  {
    if (argc == 1)
    {
      show_help();
      return 0;
    }
    if (std::string(argv[1]) == "--profile")
    { //If program has been called like './pbd --profile', do a profile run
      const parameters b = create_profile_parameters_set();
      pbd::only_sim_to_nltt_recon (b);
      return 0;
    }
    if (std::string(argv[1]) == "--create")
    {
      create();
      return 0;
    }
    //Assume the second argument, './jobo something.txt' is the filename of a parameters file.
    //Use that one to load parameters and start the sim
    const std::string filename{argv[1]};
    show_file(filename);
    assert(is_regular_file(filename));
    const parameters c = load_parameters(filename);
    std::clog << "Parameters loaded: " << c << '\n';
    pbd::only_sim_to_nltt_recon(c);
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


