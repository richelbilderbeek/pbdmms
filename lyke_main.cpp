#include <iostream>
#include <ctime>

#include "lyke_individual.h"
#include "lyke_random.h"
#include "lyke_simulation.h"
#include "lyke_utils.h"
#include "lyke_parameters.h"

//#include "pbd_helper.h"

int main(int argc, char *argv[])
{
  for (int i=0; i!=argc; ++i)
  {
    std::cout << i << ": " << argv[i] << '\n';
  }

  lyke_parameters p;
  if (argc == 2)
  {
    if (std::string(argv[1]) == std::string("--profile"))
    {
      //Use profiling default setup
      const lyke_parameters q(
        100, //simulationruns
        10, //L
        10, //nGeneEco
        0.001, //mu
        0.2, //sigmaMut
        300, //popSize
        1.0, // sigmac
        5.0, // sigmaK
        1.0, //alpha
        0.1, //beta
        42 //seed
      );
      p = q;
      g_parameters = q;
    }
    else
    {
      //Parameters are read from file, './lyke parameters.txt'
      const std::string filename = argv[1];
      p = read_parameters_from_file(filename);
      g_parameters = p;
    }
  }
  else
  {
    //Use default parameters used for profiling
    g_parameters = p;
  }

  std::ofstream time_to_runfilestream ("time.csv"); //opens excel file
  const std::time_t start_time = std::time(nullptr);
  time_to_runfilestream << "Time start of simulation:" << ',' << std::ctime(&start_time);


  {
    std::ofstream f("parameters.txt");
    f << p;
  }


  std::ofstream EcoTypeFilestream ("ecotype.csv"); //opens excel file
  std::ofstream HistogramFilestream("Histogram.csv");//opens excel file
  std::ofstream DefaultresultsFiles ("lyke_defaultresults.csv");

  //std::cout << "Setting seed to: " << seed << '\n';

  std::vector<Individual> population(p.get_popSize());

  //std::cout << "simulation started" << '\n';



  for (int i = 0; i < p.get_simulationruns(); ++i)  //number of generations
  {
    const auto next_population = create_and_log_next_generation(
      population, EcoTypeFilestream, DefaultresultsFiles
    ); // updates population

    //Overwrite the current generation with the new kids
    population = next_population;

    //std::cout << " Generation:" << i << " "; //output
    doStatistics(population);
    doHistogram(population, i+1, HistogramFilestream);

  }
  std::time_t end_time = std::time(nullptr);
  time_to_runfilestream << "Time end of simulation:" << ',' <<std::ctime(&end_time);
  HistogramFilestream.close();
}
