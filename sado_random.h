#ifndef SADO_RANDOM_H
#define SADO_RANDOM_H

namespace sado {

  extern long int idum;

  double Uniform();


  double gasdev();


  void SetSeed(int seed);


  double Normal(double mean,double stdev);

  int RandomNumber(int N);


} //~namespace sado

#endif // SADO_RANDOM_H
