#include "kewe_globals.h"

int haploid=0,diploid=1;
bigint popsize=100, endtime=1000;
double x0=0.5,p0=0.5,q0=0.5,sc=0.4,se=0.6,sk=1.2,c=0.0005,sm=0.2,sv=0.02,sq=1.0,eta=1.0,b=4,hista[histw],histbinx=0.1,histbinp=0.1,histbinq=0.1, histbina=0.1;
std::ofstream out;
std::list<indiv> pop;


