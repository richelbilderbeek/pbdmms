#include <QApplication>

#include "sado_parameters.h"
#include "sado_simulation.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  std::system("rm *.csv");

  std::string filename = "article.txt";
  if (argc == 1)
  {
    sado::create_article_file(filename);
  }
  else if (argc == 2 && std::string(argv[1]) == "--article")
  {
    sado::create_article_file(filename);
  }
  else if (argc == 2 && std::string(argv[1]) == "--golden")
  {
    sado::create_golden_standard_file(filename);
  }
  else if (argc == 2 && std::string(argv[1]) == "--profile")
  {
    sado::create_profiling_file(filename);
  }
  else
  {
    filename = std::string(argv[1]);
  }
  sado::do_simulation(filename);

  //histogram_to_png("eco_traits.csv", "eco_traits.png");
  //histogram_to_png("fem_prefs.csv", "fem_prefs.png");
  //histogram_to_png("male_traits.csv", "male_traits.png");
  return 0;
}
