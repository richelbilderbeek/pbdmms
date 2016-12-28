#include <QApplication>

#include "sado_parameters.h"
#include "sado_simulation.h"

using namespace sado;

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  std::system("rm *.csv");

  std::string filename = "article.txt";
  if (argc == 1)
  {
    simulation s(create_article_parameters());
    s.run();
  }
  else if (argc == 2 && std::string(argv[1]) == "--article")
  {
    simulation s(create_article_parameters());
    s.run();
    return 0;
  }
  else if (argc == 2 && std::string(argv[1]) == "--golden")
  {
    simulation s(create_golden_standard_parameters());
    s.run();
    return 0;
  }
  else if (argc == 2 && std::string(argv[1]) == "--profile")
  {
    simulation s(create_profiling_parameters());
    s.run();
    return 0;
  }
  else
  {
    filename = std::string(argv[1]);
  }
  simulation s(readparameters(filename));
  s.run();

  //histogram_to_png("eco_traits.csv", "eco_traits.png");
  //histogram_to_png("fem_prefs.csv", "fem_prefs.png");
  //histogram_to_png("male_traits.csv", "male_traits.png");
  return 0;
}
