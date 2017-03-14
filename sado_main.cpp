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

parameters get_parameters(const int argc, const char * const argv[])
{
  if (argc == 1) return create_article_parameters();
  if (std::string(argv[1]) == "--article") return create_article_parameters();
  if (std::string(argv[1]) == "--golden") return create_golden_standard_parameters();
  if (std::string(argv[1]) == "--profile") return create_profiling_parameters();
  return read_parameters(std::string(argv[1]));
}

int main(int argc, char *argv[])
{
  QApplication a(argc, argv); //!OCLINT a is used in the background
  try
  {
    std::setlocale(LC_ALL, "en_US.UTF-8");
    assert(std::stod("0.005") > 0.004);

    simulation s(get_parameters(argc, argv));
    s.run();

    const results res = s.get_results();
    const std::vector<species> spp = res.get_species();
    const auto g = create_graph_from_species_vector(spp);
    const auto h = create_reconstructed(g);
    save_to_png(h, "r_resultphylogeny.png");
    std::ofstream out("resultnewick");
    out << to_newick(h);

    histogram_to_png("eco_traits.csv", "eco_traits.png");
    histogram_to_png("fem_prefs.csv", "fem_prefs.png");
    histogram_to_png("male_traits.csv", "male_traits.png");
  }
  catch (std::exception& e)
  {
    std::cout << e.what() << '\n';
    return 1;
  }
}
