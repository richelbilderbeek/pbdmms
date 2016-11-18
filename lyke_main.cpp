#include <iostream>

#include "lyke_individual.h"
#include "lyke_random.h"
#include "lyke_simulation.h"
#include "lyke_utils.h"

int main()
{
  const int seed{42};
  std::cout << "Setting seed to: " << seed << '\n';
  rnd::set_seed(seed);
  Individual::init();
  for (int i = 0; i < popSize; ++i) population[i] = new Individual;//allocates storage space
  std::cout << "simulation started" << '\n';
  //std::vector <double>TempsubstitutionsXnonsynonymous((L / 2), 0);
  //Temporary vectors to store frequencies of indv of population
  //std::vector <double>TempsubstitutionsXsynonymous((L / 2), 0);
  //std::vector <double>TempsubstitutionsYnonsynonymous((L / 2), 0);
  //std::vector <double>TempsubstitutionsYsynonymous((L / 2), 0);
  EcoTypeFilestream << "Generation" << ","
                    << "Ecotype" << ","
                    << "Individual" << "\n"; //output to csv.file
  HistogramFilestream << "Time,1,2,3,4,5,6,7,8,9,10,11,12,13,14" << std::endl;
  for (int i = 0; i < static_cast<int>(simulationruns); ++i)  //number of generations
  {
    EcoTypeFilestream << 1 + i;
    iterate(); // updates population
    std::cout << " Generation:" << i << " "; //output
    //EcoTypeFilestream << "Generation" << ','
    //                  << "Average ecotype" << ','
    //                  << "Standard deviation" << '\n;
    //EcoTypeFilestream << 1 + i;
    doStatistics();
    doHistogram(i+1);
    //doSubstitutions(TempsubstitutionsXnonsynonymous, TempsubstitutionsXsynonymous,
    //TempsubstitutionsYnonsynonymous, TempsubstitutionsYsynonymous);
  }


  EcoTypeFilestream.close(); //closes excel file
  //SubstitutionFilestream.close(); //closes excel file
  HistogramFilestream.close();
  DefaultresultsFiles.close();

  for (int i = 0; i < popSize; ++i) delete population[i];
}
