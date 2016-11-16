#include<iostream>
#include<fstream>
#include<iomanip>
#include<cmath>
#include<cstdlib>
#include<cstring>
#include<list>
#include <stdexcept>
#include <numeric>
#include <limits>
#include <cassert>
#include <vector>
#include <string>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <QFile>
#pragma GCC diagnostic pop

#include "kewe_random.h"
#include "kewe_individual.h"
#include "kewe_parameters.h"
#include "kewe_results.h"
#include "kewe_SES.h"
#include "kewe_simulation.h"

inline double gauss(double xx, double sigma)
{ return exp(-(xx*xx)/(2.0*sigma*sigma));}

// Pick random individual
bigint randomindividual(const std::vector<indiv>& pop)
{
  return floor(Uniform()*static_cast<int>(pop.size()));
}

std::vector<indiv> initialize()
{
    kewe_parameters parameters; //Testing parameters by default
    const int histw = parameters.output_parameters.histw;
    std::ofstream out (parameters.output_parameters.outputfilename);
    std::vector<indiv> pop;
    bigint j;
    int k;
    indiv I(parameters);

    SetSeed(parameters.sim_parameters.seed);
    I.init(parameters);

    for(j=0;j<parameters.sim_parameters.popsize;j++)
        pop.push_back(I);

    out<<"generation,popsize,rhoxp,rhoxq,rhopq,sx,sp,sq,";

    for(k=0;k<histw;k++)
        out<<","<<(k-histw/2)*parameters.output_parameters.histbinx;
    for(k=0;k<histw;k++)
        out<<","<<(k-histw/2)*parameters.output_parameters.histbinp;
    for(k=0;k<histw;k++)
        out<<","<<(k-histw/2)*parameters.output_parameters.histbinq;
    out<<std::endl;
    return pop;
}

void iterate(
  std::vector<std::vector<double>> &histX,
  std::vector<std::vector<double>> &histP,
  std::vector<std::vector<double>> &histQ,
  const kewe_parameters& parameters,
  std::vector<indiv>& pop,
  result_variables& output_variables
)
{
    bigint j;    // iterates through a vector/list (Keeps track of the individual
    indiv kid(parameters);  // potential baby
    bigint k,t;
    double nkid,comp,xi,pi,qi,xj,qj,attractiveness,draw;
    for(t=0;t<=parameters.sim_parameters.endtime && static_cast<bigint>(pop.size()) != 0;t++)
    {
        if(t%parameters.output_parameters.outputfreq==0)
          output(t, histX, histP, histQ, parameters, pop, output_variables); // Output once every outputfreq

        for(k=0;k<static_cast<bigint>(pop.size()) && static_cast<bigint>(pop.size())  != 0;k++)
        {
            // Pick random individual and get its x, p and q loci
            const auto i = randomindividual(pop);
            xi=pop[i]._x();
            pi=pop[i]._p();
            qi=pop[i]._q();

            comp=0.0; // competition

            for(j=0;j < static_cast<bigint>(pop.size());++j) // Go through all individuals
            {
                if(j != i)
                {
                    xj= pop[j]._x();          // As long as J is not I, make xj j's x.
                    comp+=gauss(xi-xj,parameters.sim_parameters.sc);  // Add intensity competition
                }
            }

            // If individual survives calculate its attractiveness
            if(Uniform() < (
              1.0 - comp * parameters.sim_parameters.c
              / gauss(xi,parameters.sim_parameters.sk))
              * (0.5+0.5*gauss(qi,parameters.sim_parameters.sq)))
            // 1.0 - comp ... ...sq))) == survival rate
            {
              attractiveness=parameters.sim_parameters.eta;
              for(j=0;j < static_cast<bigint>(pop.size());++j)
              {
                if(j!=i)
                {
                    qj=pop[j]._q();
                    xj=pop[j]._x();
                    attractiveness+=gauss(
                      pi-qj,
                      parameters.sim_parameters.sm
                    )*gauss(xi-xj,parameters.sim_parameters.se);
                    // gauss ... ... se) == A ik -->
                    //attractivenes == formula[2] under the devision line.

                    pop[j].a_(attractiveness); //set j's a to attractiveness.
                }
              }

              for(nkid=0.0;;nkid+=1.0)
              {
                if(Uniform()>=parameters.sim_parameters.b-nkid) break; // have max b kids

                draw=Uniform()*attractiveness;
                if(draw>parameters.sim_parameters.eta) // can female find an attractive male?
                {
                  for(j=0;j < static_cast<bigint>(pop.size());++j) // Go through all individuals
                  {
                    if(j!=i && draw<=pop[j]._a()) // if male is attractive enough
                    {
                      kid.birth((pop[i]),(pop[j]), parameters); // i and j make baby
                      pop.push_back(kid); // add kid to population
                      break; // stop looking for mate
                    }
                  }
                }
              }
            }
            // Then kill the individual (females die after mating)
            //pop.erase(pop.begin()+i);
            assert(!pop.empty());
            std::swap(*(pop.begin()+i), pop.back());
            pop.pop_back();
        }
    }
    return;
}

