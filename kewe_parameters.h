#ifndef KEWE_PARAMETERS_H
#define KEWE_PARAMETERS_H

struct kewe_parameters
{
  int Nx = 2; //Number of X alleles
  int Np = 2; //Number of P alleles
  int Nq = 2; //Number of Q alleles
  int seed = 123; //Seed for RNG
};

#endif // KEWE_PARAMETERS_H
