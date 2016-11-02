#ifndef KEWE_PARAMETERS_H
#define KEWE_PARAMETERS_H

#include <string>

struct kewe_parameters
{
  int Nx = 2; //Number of X alleles
  int Np = 2; //Number of P alleles
  int Nq = 2; //Number of Q alleles
  int seed = 123; //Seed for RNG
  int outputfreq = 10; //output frequency
  int haploid = 1; // 1 == run simulation haploid
  int diploid = 0; // 1 == run simulation diploid
  long unsigned int popsize = 100; // Initial population size
  std::string outputfilename = "defaultresults";
};

#endif // KEWE_PARAMETERS_H
