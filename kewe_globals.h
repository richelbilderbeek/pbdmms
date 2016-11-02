#ifndef KEWE_GLOBALS_H
#define KEWE_GLOBALS_H

#include <fstream>
#include <list>
#include "kewe_individual.h"
typedef long unsigned int bigint;

const int histw=50;

extern int seed;
extern int outputfreq;
extern int haploid;
extern int diploid;
extern bigint popsize;
extern bigint endtime;
extern double x0;
extern double p0;
extern double q0;
extern double sc;
extern double se;
extern double sk;
extern double c;
extern double sm;
extern double sv;
extern double  sq;
extern double eta;
extern double b;
extern double  hista[histw];
extern double histbinx;
extern double histbinp;
extern double histbinq;
extern double  histbina;
extern std::ofstream out;
extern std::list<indiv> pop;


#endif // KEWE_GLOBALS_H
