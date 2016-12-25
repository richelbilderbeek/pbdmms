#include <QApplication>

#include "sado_simulation.h"
#include "kewe_parameters.h"
#include "histogram_to_png.h"

void create_testrun_file(const std::string& filename)
{
  std::ofstream f(filename);
  f
    << "alleles 2 2 2\n"
    << "histbin 0.1 0.1 0.1 0.1\n"
    << "seed 123\n"
    << "pop0 100\n"
    << "type0 0.5 0.5 0.5\n"
    << "end 4000\n"
    << "sc 0.3\n"
    << "se 0.1\n"
    << "sk 1.2\n"
    << "c 0.01\n"
    << "sm 0.1\n"
    << "sv 0.02\n"
    << "sq 1.0\n"
    << "eta 1.0\n"
    << "b 4.0\n"
    << "output 10 output.txt\n"
    << "diploid 1\n"
  ;
}

void create_article_file(const std::string& filename)
{
  std::ofstream f(filename);
  f
    << "alleles 1 1 1\n"
    << "histbin 0.1 0.1 0.1 0.1\n"
    << "seed 123\n"
    << "pop0 1000\n"
    << "type0 0.5 0.5 0.5\n"
    << "end 4000\n"
    << "sc 0.4\n"
    << "se 0.6\n"
    << "sk 1.2\n"
    << "c 0.0005\n"
    << "sm 0.2\n"
    << "sv 0.02\n"
    << "sq 1.0\n"
    << "eta 1.0\n"
    << "b 4.0\n"
    << "output 10 output.txt\n"
    << "haploid 1\n"
  ;
}

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  std::system("rm *.csv");

  std::string filename = "article.txt";
  if (argc == 1)
  {
    create_article_file(filename);
  }
  else
  {
    filename = std::string(argv[1]);
  }

  sado::do_simulation(filename);

  histogram_to_png("eco_traits.csv", "eco_traits.png");
  histogram_to_png("fem_prefs.csv", "fem_prefs.png");
  histogram_to_png("male_traits.csv", "male_traits.png");

  return a.exec();
}
