#include <cassert>
#include <algorithm>
#include <QApplication>
#include "histogram_to_png.h"
#include "sado_parameters.h"
#include "sado_simulation.h"
#include "sado_ancestry_graph.h"
#include "save_graph_to_dot.h"
#include "convert_dot_to_svg.h"
#include "convert_svg_to_png.h"
#include "sado_newick.h"
#include "sado_likelihood.h"

using namespace sado;

parameters get_base_parameters(const int argc, const char * const argv[])
{
  if (argc == 1) return create_article_parameters();
  const std::string argument{argv[1]};
  if (argument == "--article") return create_article_parameters();
  if (argument == "--golden") return create_golden_standard_parameters();
  if (argument == "--profile") return create_profiling_parameters();
  return read_parameters(argument);
}

bool has_arg(const int argc, const char * const argv[], const std::string& s)
{
  for (int i=0; i!=argc; ++i)
  {
    if (argv[i] == s) return true;
  }
  return false;
}

bool get_verbosity(const int argc, const char * const argv[])
{
  return has_arg(argc, argv, "--verbose");
}

parameters get_parameters(const int argc, const char * const argv[])
{
  if (get_verbosity(argc, argv)) std::clog << "Reading parameters" << '\n';

  parameters p = get_base_parameters(argc, argv);
  for (int i=0; i!=argc-1; ++i)
  {
    if (argv[i] == std::string("c")) p.set_c(std::stod(argv[i + 1]));
    if (argv[i] == std::string("end")) p.set_end(std::stoi(argv[i + 1]));
  }

  if (get_verbosity(argc, argv)) std::clog << p << '\n';

  return p;
}

bool do_ml(const int argc, const char * const argv[])
{
  return !has_arg(argc, argv, "--no_ml");
}

bool save_full_tree(const int argc, const char * const argv[])
{
  return has_arg(argc, argv, "--save_full_tree");
}

bool save_reconstructed_tree(const int argc, const char * const argv[])
{
  return has_arg(argc, argv, "--save_reconstructed_tree");
}

bool do_sim_only(const int argc, const char * const argv[])
{
  return has_arg(argc, argv, "--sim_only");
}

bool no_reconstructed_tree(const int argc, const char * const argv[])
{
  return has_arg(argc, argv, "--no_reconstructed_tree");
}

void do_ml(const std::string& newick, const int argc, char * argv[])
{
  if (do_ml(argc, argv))
  {
    if (get_verbosity(argc, argv)) std::clog << "Do maximum likelihood analysis" << '\n';
    if (newick == "")
    {
      std::clog << "maximum likelihood analysis:\n"<< "NA" << '\n';
    }
    else
    {
      const auto likelihood = calc_max_likelihood(newick);
      std::clog << "maximum likelihood analysis:\n"<< likelihood << '\n';
    }
  }
}

void create_histograms(const int argc, char * argv[])
{
  if (get_verbosity(argc, argv)) std::clog << "Create histograms" << '\n';
  histogram_to_png("eco_traits.csv", "eco_traits.png");
  histogram_to_png("fem_prefs.csv", "fem_prefs.png");
  histogram_to_png("male_traits.csv", "male_traits.png");
}

void save_full_tree(const ancestry_graph& g, const int argc, char * argv[])
{
  if (save_full_tree(argc, argv))
  {
    if (get_verbosity(argc, argv)) std::clog << "Save full tree" << '\n';
    save_to_png(g, "tree_full.png");
  }
}

void close()
{
  qApp->processEvents();
  qApp->closeAllWindows();
  qApp->exit(0);
}

simulation get_run_simulation(const parameters& p, const int argc, char * argv[])
{
  simulation s(p);
  if (get_verbosity(argc, argv)) std::clog << "Start simulation" << '\n';
  s.run();
  return s;
}

results get_results(const simulation& s, const int argc, char * argv[])
{
  if (get_verbosity(argc, argv)) std::clog << "Getting results" << '\n';
  return s.get_results();
}

ancestry_graph create_ancestry_graph(const results &r, const int argc, char * argv[])
{
  if (get_verbosity(argc, argv)) std::clog << "create_graph_from_species_vector" << '\n';
  return create_ancestry_graph(r);
}

int main(int argc, char *argv[])
{
  QApplication a(argc, argv); //!OCLINT a is used in the background
  std::setlocale(LC_ALL, "en_US.UTF-8");
  assert(std::stod("0.005") > 0.004);

  const bool verbose{get_verbosity(argc, argv)};

  try
  {
    const parameters p{get_parameters(argc, argv)};

    const simulation s{get_run_simulation(p, argc, argv)};

    if (do_sim_only(argc, argv))
    {
      return 0;
    }

    const results r = get_results(s, argc, argv);

    const auto g = create_ancestry_graph(r, argc, argv);

    save_full_tree(g, argc, argv); //If wanted

    create_histograms(argc, argv);


    if (no_reconstructed_tree(argc, argv))
    {
      return 0;
    }

    if (verbose) std::clog << "Create reconstructed tree" << '\n';
    const auto h = create_reconstructed(g);

    if (save_reconstructed_tree(argc, argv))
    {
      if (verbose) std::clog << "Saving reconstructed tree" << '\n';
      save_to_png(h, "tree_reconstructed.png");
    }

    if (verbose) std::clog << "Constructing newick" << '\n';
    const auto newick = to_newick(h);
    std::clog << "reconstructed tree:\n" << newick << '\n';

    do_ml(newick, argc, argv);
  }
  catch (std::exception& e)
  {
    std::clog << e.what() << '\n';
    return 1;
  }
  close();
}
