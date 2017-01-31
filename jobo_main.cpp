#include "jkr_experiment.h"
#include "jobo_jkr_adapters.h"
#include "jobo_parameters.h"
#include "jobo_simulation.h"
#include "jobo_results.h"
#include <cassert>
#include <algorithm>
#include <exception>
#include <iostream>
#include <iterator>
#include <fstream>
#include "jobo_parameters.h"
#include "is_regular_file.h"

void run(const jobo::parameters& p)
{
  std::clog << "Parameters loaded: " << p << '\n';
  jobo::simulation s(p);
  s.run();
  const auto& r = s.get_results();
  save_ltt_plot_viables(r, p.get_ltt_plot_filename_vi());
  save_ltt_plot_inviables(r, p.get_ltt_plot_filename_in());
  save_nltt_plot_viables(r, p.get_nltt_plot_filename_v());
  save_nltt_plot_inviables(r, p.get_nltt_plot_filename_i());
}

void run_from_args(const std::vector<std::string>& args)
{
  assert(args.size() == 9);
  const jobo::parameters p(
    std::stoi(args[0]), //const int population_size,
    std::stoi(args[1]), //const int seed,
    std::stod(args[2]), //const double mutation_rate,
    std::stoi(args[3]), //const int n_generations,
    std::stoi(args[4]), //const int n_loci,
    args[5], //const std::string& ltt_plot_filename_vi = "jobo_ltt_viables.csv",
    args[6], //const std::string& ltt_plot_filename_in = "jobo_ltt_inviables.csv",
    args[7], //const std::string& nltt_plot_filename_v = "jobo_nltt_viables.csv",
    args[8]  //const std::string& nltt_plot_filename_i = "jobo_nltt_inviables.csv");
  );
  run(p);
}

void create() noexcept
{
  const std::string filename{"jobo.txt"};
  if (!is_regular_file(filename))
  {
    std::ofstream f(filename);
    f << jobo::create_test_parameters_1();
  }
}
void run_profile()
{
  run(jobo::create_profiling_parameters());
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

void show_help() noexcept
{
  std::cout
    << "jobo" << '\n'
    << "----" << '\n'
    << "" << '\n'
    << "Run a jobo/BDM simulation" << '\n'
    << "" << '\n'
    << "Use: " << '\n'
    << "" << '\n'
    << "  Run a jobo simulation,"<< '\n'
    << "    with parameter filename 'jobo.txt':" << '\n'
    << "" << '\n'
    << "    ./jobo jobo.txt" << '\n'
    << "" << '\n'
    << "  Run a PBD simulation,"<< '\n'
    << "    from parameters:" << '\n'
    << "" << '\n'
    << "    ./jobo [p_sz] [s] [m] [n_gen] [n_loc] [lvfn] [lifn] [nvfn] [nifn]" << '\n'
    << "    ./jobo 30 42 0.1 20 10 ltt_v.csv ltt_i.csv nltt_v.csv nltt_i.csv" << '\n'
    << "" << '\n'
    << "    [p_sz]: population_size" << '\n'
    << "    [s]: random number generator seed" << '\n'
    << "    [m]: mutation rate, probability per locus" << '\n'
    << "    [n_gen]: number of generations" << '\n'
    << "    [n_loc]: number of loci" << '\n'
    << "    [lvfn]: filename for LTT plot of viable species" << '\n'
    << "    [lifn]: filename for LTT plot of inviable species" << '\n'
    << "    [nvfn]: filename for nLTT plot of viable species" << '\n'
    << "    [nifn]: filename for nLTT plot of inviable species" << '\n'
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


void run_from_file(const std::string& filename)
{
  run(jobo::load_parameters(filename));
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
    if (args.size() == 9)
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
