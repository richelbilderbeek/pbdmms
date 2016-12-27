#ifndef SADO_GLOBALS_H
#define SADO_GLOBALS_H

#include <fstream>

namespace sado
{

  typedef long unsigned int bigint;
  const int histw{50};
  extern int seed;
  extern int outputfreq;
  extern int endtime;
  extern double x0;
  extern double p0;
  extern double q0;
  extern double sc;
  extern double se;
  extern double sk;
  extern double c;
  extern double sm;
  extern double sv;
  extern double sq;
  extern double eta;
  extern double b;
  extern double histx[histw];
  extern double histp[histw];
  extern double histq[histw];
  extern double histbinx;
  extern double histbinp;
  extern double histbinq;
  extern std::ofstream out;


} //~namespace sado

#endif // SADO_GLOBALS_H
