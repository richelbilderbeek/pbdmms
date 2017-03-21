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

parameters get_parameters(const int argc, const char * const argv[])
{
  parameters p = get_base_parameters(argc, argv);
  for (int i=0; i!=argc-1; ++i)
  {
    if (argv[i] == std::string("c")) p.set_c(std::stod(argv[i + 1]));
    if (argv[i] == std::string("end")) p.set_end(std::stoi(argv[i + 1]));
  }
  return p;
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


int main(int argc, char *argv[])
{
  QApplication a(argc, argv); //!OCLINT a is used in the background
  std::setlocale(LC_ALL, "en_US.UTF-8");
  assert(std::stod("0.005") > 0.004);

  const bool verbose{get_verbosity(argc, argv)};

  try
  {
    if (verbose) std::clog << "Reading parameters" << '\n';
    const parameters p{get_parameters(argc, argv)};
    if (verbose) std::clog << p << '\n';
    simulation s(p);
    if (verbose) std::clog << "Start simulation" << '\n';
    s.run();
    if (verbose) std::clog << "Getting results" << '\n';
    const results res = s.get_results();
    const std::vector<species> spp = res.get_species();

    if (verbose) std::clog << "create_graph_from_species_vector" << '\n';
    const auto g = create_ancestry_graph(spp);

    if (save_full_tree(argc, argv))
    {
      if (verbose) std::clog << "Save full tree" << '\n';
      save_to_png(g, "tree_full.png");
    }

    if (verbose) std::clog << "Create histograms" << '\n';
    histogram_to_png("eco_traits.csv", "eco_traits.png");
    histogram_to_png("fem_prefs.csv", "fem_prefs.png");
    histogram_to_png("male_traits.csv", "male_traits.png");

    if (verbose) std::clog << "Create reconstructed tree" << '\n';
    const auto h = create_reconstructed(g);

    if (save_reconstructed_tree(argc, argv))
    {
      if (verbose) std::clog << "Saving reconstructed tree" << '\n';
      save_to_png(h, "tree_reconstructed.png");
    }

    {
      if (verbose) std::clog << "Constructing newick" << '\n';
      const auto newick = to_newick(h);
      std::ofstream out("resultnewick");
      std::clog << "reconstucted tree:\n" << newick << '\n';
      out << "reconstucted tree:\n" << newick << '\n';

      if (do_ml(argc, argv))
      {
        if (verbose) std::clog << "Do maximum likelihood analysis" << '\n';
        if (newick == "")
        {
          std::clog << "maximum likelihood analysis:\n"<< "NA" << '\n';
          out << "maximum likelihood analysis:\n"<< "NA" << '\n';
        }
        else
        {
          const auto likelihood = calc_max_likelihood(newick);
          std::clog << "maximum likelihood analysis:\n"<< likelihood << '\n';
          out << "maximum likelihood analysis:\n"<< likelihood << '\n';

        }
      }
    }
  }
  catch (std::exception& e)
  {
    std::clog << e.what() << '\n';
    return 1;
  }
}
