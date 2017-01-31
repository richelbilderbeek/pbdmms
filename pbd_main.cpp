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

void create() noexcept
{
  const std::string filename{"pbd.txt"};
  if (!is_regular_file(filename))
  {
    std::ofstream f(filename);
    f << create_parameters_set1();
  }
}

void run(const parameters& p, const std::string& csv_output_filename)
{
  std::clog << "Parameters loaded: " << p << '\n';
  pbd::sim_to_nltt_igtree_extinct(p, csv_output_filename);
}


void run(const parameters& p)
{
  return run(p, "sim_to_nltt_igtree_extinct.csv");
}

void run_from_args(const std::vector<std::string>& args)
{
  assert(args.size() == 8);
  const parameters c(
    std::stod(args[0]), //birth_good,
    std::stod(args[1]), //birth_incipient,
    std::stod(args[2]), //completion,
    std::stod(args[3]), //death_good,
    std::stod(args[4]), //death_incipient,
    std::stod(args[5]), //time,
    std::stoi(args[6])  //seed,
  );
  run(c, args[7]);
}

void run_from_file(const std::string& filename)
{
  if (!is_regular_file(filename))
  {
    throw std::invalid_argument("File is absent");
  }
  run(load_parameters(filename));
}

void run_profile()
{
  run(create_profile_parameters_set());
}


void show_error(const std::vector<std::string>& args)
{
  std::cout << "invalid arguments" << '\n';
  std::cout << "arguments given (n = " << args.size() << "):\n";
  std::copy(
    std::begin(args), std::end(args),
    std::ostream_iterator<std::string>(std::cout, "\n"));
  std::cout << '\n';
  std::cout << '\n';
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
    << "  Run a PBD simulation,"<< '\n'
    << "    from parameters:" << '\n'
    << "" << '\n'
    << "    ./pbd [b_g] [b_i] [scr] [e_g] [e_i] [t] [s] [of]" << '\n'
    << "    ./pbd 0.5 0.4 0.3 0.2 0.1 1.0 42 result.csv" << '\n'
    << "" << '\n'
    << "    [b_g]: birth rate of good species, chance per mya, per lineage" << '\n'
    << "    [b_i]: birth rate of incipient species, chance per mya, per lineage" << '\n'
    << "    [scr]: speciation completion rate, chance per mya, per lineage" << '\n'
    << "    [e_g]: extinction rate of good species, chance per mya, per lineage" << '\n'
    << "    [e_i]: extinction rate of incipient species, chance per mya, per lineage" << '\n'
    << "    [t]: simulation dutation, mya" << '\n'
    << "    [s]: random number generator seed" << '\n'
    << "    [of]: output filename" << '\n'
    << "" << '\n'
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

void show_version() noexcept
{
  std::cout
    << "date: " << __DATE__ << '\n'
    << "time: " << __TIME__ << '\n'
  ;
}

int run_one_arg(const std::string arg)
{
  if (arg == "--profile")
  {
    //If program has been called like './pbd --profile', do a profile run
    run_profile();
    return 0;
  }
  if (arg == "--create")
  {
    create();
    return 0;
  }
  if (arg == "--version")
  {
    show_version();
    return 0;
  }
  //Assume it is a filename
  run_from_file(arg);
  return 0;
}

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
    if (args.empty() || args[0] == "--help")
    {
      show_help();
      return 0;
    }
    if (args.size() == 1)
    {
      return run_one_arg(args[0]);
    }
    if (args.size() == 8)
    {
      run_from_args(args);
      return 0;
    }
    show_error(args);
    return 1;
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


