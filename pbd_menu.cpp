#include "pbd_menu.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <sstream>

#include "pbd.h"
#include "pbd_parameters.h"
#include "pbd_helper.h"

pbd::menu::menu(const std::vector<std::string>& args)
  : m_args{args}
{

}

void pbd::create()
{
  const std::string filename{"pbd.txt"};
  if (is_regular_file(filename))
  {
    throw std::runtime_error("Parameter file 'pbd.txt' already present");
  }
  std::ofstream f(filename);
  f << create_parameters_set1();
}

void pbd::menu::run()
{
  if (m_args.empty() || m_args[0] == "--help")
  {
    show_help();
    return;
  }
  if (m_args.size() == 1)
  {
    run_one_arg(m_args[0]);
    return;
  }
  if (m_args.size() == 8)
  {
    run_from_args(m_args);
    return;
  }

  std::stringstream msg;
  msg
    << "invalid arguments" << '\n'
    << "arguments given (n = " << m_args.size() << "):\n"
  ;
  std::copy(
    std::begin(m_args), std::end(m_args),
    std::ostream_iterator<std::string>(msg, "\n"));
  throw std::invalid_argument(msg.str());
}

void pbd::run_from_args(const std::vector<std::string>& args)
{
  assert(args.size() == 8);
  const parameters c(
    std::stod(args[0]), //birth_good,
    std::stod(args[1]), //birth_incipient,
    std::stod(args[2]), //completion,
    std::stod(args[3]), //death_good,
    std::stod(args[4]), //death_incipient,
    std::stod(args[5]), //time,
    std::stoi(args[6]), //seed,
    args[7] //pbd_nltt_filename
  );
  run_sim(c);
}

void pbd::run_from_file(const std::string& filename)
{
  if (!is_regular_file(filename))
  {
    throw std::invalid_argument("File is absent");
  }
  run_sim(load_parameters(filename));
}

void pbd::run_one_arg(const std::string& arg)
{
  if (arg == "--profile")
  {
    //If program has been called like './pbd --profile', do a profile run
    run_profile();
    return;
  }
  if (arg == "--create")
  {
    create();
    return;
  }
  if (arg == "--version")
  {
    show_version();
    return;
  }
  //Assume it is a filename
  run_from_file(arg);
}

void pbd::run_profile()
{
  run_sim(create_profile_parameters_set());
}

void pbd::run_sim(const parameters& p)
{
  std::clog << "Parameters loaded: " << p << '\n';
  pbd::sim_to_nltt_igtree_extinct_to_file(p);
}




/*
void show_file(const std::string& filename)
{
  std::cout << "Showing the file:\n";
  for (const std::string& line: file_to_vector(filename))
  {
    std::cout << line << '\n';
  }
  std::cout << "Done showing the file.\n";
}
*/

void pbd::show_help() noexcept
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

void pbd::show_version() noexcept
{
  std::cout
    << "date: " << __DATE__ << '\n'
    << "time: " << __TIME__ << '\n'
  ;
}

