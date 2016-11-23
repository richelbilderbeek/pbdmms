#include <iostream>

#include "lyke_individual.h"
#include "lyke_random.h"
#include "lyke_simulation.h"
#include "lyke_utils.h"

int main()
{
  std::ofstream EcoTypeFilestream ("ecotype.csv"); //opens excel file
  const int seed{42};
  std::cout << "Setting seed to: " << seed << '\n';
  rnd::set_seed(seed);

  std::vector<Individual> population(popSize);

  std::cout << "simulation started" << '\n';

 /* EcoTypeFilestream << "Generation" << ","
                    << "Ecotype" << ","
                    << "Individual" << "\n"; //output to csv.file*/
  HistogramFilestream << "Time,1,2,3,4,5,6,7,8,9,10,11,12,13,14" << '\n';



  for (int i = 0; i < static_cast<int>(simulationruns); ++i)  //number of generations
  {
    //EcoTypeFilestream << 1 + i;
    iterate(population, EcoTypeFilestream); // updates population
    std::cout << " Generation:" << i << " "; //output
    doStatistics(population);
    doHistogram(population, i+1);

  }


  //EcoTypeFilestream.close(); //closes excel file

  HistogramFilestream.close();
  DefaultresultsFiles.close();
}
