#ifndef KEWE_PARAMETERS_H
#define KEWE_PARAMETERS_H

#include <string>

typedef long unsigned int bigint;

struct kewe_parameters
{
  bigint endtime = 1000; // End simulation at this generation

  int Nx = 2; // Number of X alleles
  int Np = 2; // Number of P alleles
  int Nq = 2; // Number of Q alleles

  double x0 = 0.5; // initial X gene
  double p0 = 0.5; // initial p gene
  double q0 = 0.5; // initial q gene

  double sc = 0.4; //?? TODO Look up sc
  double se = 0.6; //??      Look up se
  double sk = 1.2; //??      Look up sk
  double sq = 1.0; //??      Look up sq
  double sm = 0.2; //??      Look up sm
  double sv = 0.02;//??      Look up sv

  double c = 0.0005; // constant to stabalize populationsize

  double eta = 1.0; //?? TODO Look up eta
  double b = 4; //number of babies

  int seed = 123;                                 // Seed for RNG
  int outputfreq = 10;                            // output frequency
  int haploid = 1;                                // 1 == run simulation haploid
  int diploid = 0;                                // 1 == run simulation diploid
  long unsigned int popsize = 100;                // Initial population size
  int histw = 50;                                 // Number of steps in the histograms
  double histbinx = 0.1;                          // stepsize of histogram x
  double histbinp = 0.1;                          // stepsize of histogram p
  double histbinq = 0.1;                          // stepsize of histogram q
  std::string outputfilename = "defaultresults";
};

#endif // KEWE_PARAMETERS_H
