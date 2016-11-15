#include "ribi_menu_dialog.h"

#include <cassert>
#include <iostream>

#include "is_regular_file.h"
#include "ribi_parameters.h"
#include "ribi_simulation.h"
#include "ribi_results.h"

int ribi::menu_dialog::execute(const std::vector<std::string>& args)
{
  if (args.empty() || args.size() == 1)
  {
    show_help();
    return 0;
  }
  assert(args.size() >= 2);
  if (args[1] == "--demo")
  {
    run_demo();
    return 0;
  }
  if (args[1] == "--profile")
  {
    run_profile();
    return 0;
  }
  if (is_regular_file(args[1]))
  {
    run_from_file(args[1]);
    return 0;
  }
  show_help();
  return 0;
}

void ribi::menu_dialog::show_help() noexcept
{
  std::cout
    << "Usages:\n"
    << '\n'
    << "Run a simulation from a parameter file:\n"
    << '\n'
    << "ribi [filename]\n"
    << '\n'
    << "Run a demo simulation\n"
    << '\n'
    << "ribi --demo\n"
    << '\n'
  ;
}

void ribi::menu_dialog::run(const parameters& p)
{
  simulation s(p);
  s.run();
  results r = s.get_results();
  r.summarize_sil_frequency_phylogeny();
  try
  {
    r.save(p.get_filename_genotype_frequency_graph());
  }
  catch (std::exception& e)
  {
    std::clog << e.what() << '\n';
  }
}

void ribi::menu_dialog::run_from_file(const std::string& parameters_filename)
{
  assert(is_regular_file(parameters_filename));
  const parameters p = load_parameters(parameters_filename);
  run(p);
}

void ribi::menu_dialog::run_demo()
{
  const parameters p = create_test_parameters_1();
  run(p);
}

void ribi::menu_dialog::run_profile()
{
  const parameters p = create_profiling_parameters();
  run(p);
}
