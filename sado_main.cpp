#include "histogram_to_png.h"
#include "sado_parameters.h"
#include "sado_simulation.h"
#include "sado_species_graph.h"
#include "save_graph_to_dot.h"
#include "convert_dot_to_svg.h"
#include "convert_svg_to_png.h"
#include "sado_newick.h"
#include <QApplication>
#include <cassert>

using namespace sado;

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  std::setlocale(LC_ALL, "en_US.UTF-8");
  assert(std::stod("0.005") > 0.004);

  if (argc == 1)
  {
    simulation s(create_article_parameters());
    s.run();
    const results res = s.get_results();
    const std::vector<species> spp = res.get_species();
    const auto g = create_graph_from_species_vector(spp);
    const auto h = create_reconstructed(g);
    save_graph_to_dot(h, "r_resultphylogeny.dot");
    convert_dot_to_svg("r_resultphylogeny.dot", "r_resultphylogeny.svg");
    convert_svg_to_png("r_resultphylogeny.svg", "r_resultphylogeny.png");
    std::ofstream out("resultnewick");
    out << to_newick(h);
    return 0;
  }
  if (argc == 2 && std::string(argv[1]) == "--article")
  {
    simulation s(create_article_parameters());
    s.run();
    histogram_to_png("eco_traits.csv", "eco_traits.png");
    histogram_to_png("fem_prefs.csv", "fem_prefs.png");
    histogram_to_png("male_traits.csv", "male_traits.png");
    return 0;
  }
  if (argc == 2 && std::string(argv[1]) == "--golden")
  {
    simulation s(create_golden_standard_parameters());
    s.run();
    histogram_to_png("eco_traits.csv", "eco_traits.png");
    histogram_to_png("fem_prefs.csv", "fem_prefs.png");
    histogram_to_png("male_traits.csv", "male_traits.png");
    return 0;
  }
  if (argc == 2 && std::string(argv[1]) == "--profile")
  {
    simulation s(create_profiling_parameters());
    s.run();
    return 0;
  }
  const std::string filename{std::string(argv[1])};
  simulation s(read_parameters(filename));
  s.run();
  histogram_to_png("eco_traits.csv", "eco_traits.png");
  histogram_to_png("fem_prefs.csv", "fem_prefs.png");
  histogram_to_png("male_traits.csv", "male_traits.png");
  return 0;
}
