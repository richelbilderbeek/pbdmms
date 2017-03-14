#include "histogram_to_png.h"
#include "sado_parameters.h"
#include "sado_simulation.h"
#include "sado_species_graph.h"
#include "save_graph_to_dot.h"
#include "convert_dot_to_svg.h"
#include "convert_svg_to_png.h"
#include "sado_newick.h"
#include "sado_likelihood.h"
#include <QApplication>
#include <cassert>

using namespace sado;

parameters get_parameters(const int argc, const char * const argv[])
{
  if (argc == 1) return create_article_parameters();
  const std::string argument{argv[1]};
  if (argument == "--article") return create_article_parameters();
  if (argument == "--golden") return create_golden_standard_parameters();
  if (argument == "--profile") return create_profiling_parameters();
  return read_parameters(argument);
}

int main(int argc, char *argv[])
{
  QApplication a(argc, argv); //!OCLINT a is used in the background
  std::setlocale(LC_ALL, "en_US.UTF-8");
  assert(std::stod("0.005") > 0.004);

  try
  {

    simulation s(get_parameters(argc, argv));
    s.run();

    const results res = s.get_results();
    const std::vector<species> spp = res.get_species();
    const auto g = create_graph_from_species_vector(spp);
    save_to_png(g, "tree_full.png");
    histogram_to_png("eco_traits.csv", "eco_traits.png");
    histogram_to_png("fem_prefs.csv", "fem_prefs.png");
    histogram_to_png("male_traits.csv", "male_traits.png");

    const auto h = create_reconstructed(g);
    save_to_png(h, "tree_reconstructed.png");
    //save_to_png(h, "r_resultphylogeny.png");
    {
      std::ofstream out("resultnewick");
      const auto newick = to_newick(h);
      std::cout << "reconstucted tree:\n" << newick << '\n';
      out << "reconstucted tree:\n" << newick << '\n';
      const auto likelihood = calc_max_likelihood(newick);
      std::cout << "maximum likelihood analysis:\n"<< likelihood << '\n';
      out << "maximum likelihood analysis:\n"<< likelihood << '\n';
    }
  }
  catch (std::exception& e)
  {
    std::cout << e.what() << '\n';
    return 1;
  }
}
