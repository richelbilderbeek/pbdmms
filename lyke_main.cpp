#include <iostream>
#include <ctime>
#include "lyke_individual.h"
#include "lyke_random.h"
#include "lyke_simulation.h"
#include "lyke_utils.h"
#include "lyke_parameters.h"


int main()
{
  std::time_t start_time = std::time(nullptr);
  std::cout << "Time start of simulation:" << std::ctime(&start_time);

  lyke_parameters p;

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
  std::cout << "Time end of simulation:" << std::ctime(&end_time);
}
