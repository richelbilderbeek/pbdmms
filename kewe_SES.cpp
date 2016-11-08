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

#include <QFile>

#include "kewe_random.h"
#include "kewe_individual.h"
#include "kewe_parameters.h"
#include "kewe_results.h"
#include "kewe_SES.h"

void create_test_parameter_file(const std::string& filename)
{
    std::ofstream f(filename.c_str());
    f << "STUB";
}


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
    const int histw = parameters.histw;
    std::ofstream out (parameters.outputfilename);
    std::vector<indiv> pop;
    bigint j;
    int k;
    indiv I(parameters);

    SetSeed(parameters.seed);
    I.init(parameters);

    for(j=0;j<parameters.popsize;j++)
        pop.push_back(I);

    out<<"generation,popsize,rhoxp,rhoxq,rhopq,sx,sp,sq,";

    for(k=0;k<histw;k++)
        out<<","<<(k-histw/2)*parameters.histbinx;
    for(k=0;k<histw;k++)
        out<<","<<(k-histw/2)*parameters.histbinp;
    for(k=0;k<histw;k++)
        out<<","<<(k-histw/2)*parameters.histbinq;
    out<<std::endl;
    return pop;
}

void iterate(
  std::vector<std::vector<double>> &histX,
  std::vector<std::vector<double>> &histP,
  std::vector<std::vector<double>> &histQ,
  const kewe_parameters& parameters,
  std::vector<indiv>& pop
)
{
    bigint j;    // iterates through a vector/list (Keeps track of the individual
    indiv kid(parameters);  // potential baby
    bigint k,t;
    double nkid,comp,xi,pi,qi,xj,qj,attractiveness,draw;

    for(t=0;t<=parameters.endtime && static_cast<bigint>(pop.size()) != 0;t++)
    {
        if(t%parameters.outputfreq==0) output(t, histX, histP, histQ, parameters, pop); // Output once every outputfreq

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
                    comp+=gauss(xi-xj,parameters.sc);  // Add intensity competition
                }
            }

            // If individual survives calculate its attractiveness
            if(Uniform() < (1.0 - comp * parameters.c / gauss(xi,parameters.sk)) * (0.5+0.5*gauss(qi,parameters.sq)))
            // 1.0 - comp ... ...sq))) == survival rate
            {
                attractiveness=parameters.eta;
                for(j=0;j < static_cast<bigint>(pop.size());++j)
                {
                    if(j!=i)
                    {
                        qj=pop[j]._q();
                        xj=pop[j]._x();
                        attractiveness+=gauss(pi-qj,parameters.sm)*gauss(xi-xj,parameters.se);
                        // gauss ... ... se) == A ik --> attractivenes == formula[2] under the devision line.
                        pop[j].a_(attractiveness); //set j's a to attractiveness.
                    }
                }

                for(nkid=0.0;;nkid+=1.0)
                {
                    if(Uniform()>=parameters.b-nkid) break; // have max b kids

                    draw=Uniform()*attractiveness;
                    if(draw>parameters.eta) // can female find an attractive male?
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

kewe_parameters readparameters(const std::string& filename)
{
    kewe_parameters parameters;
    //parameters.set_n_alleles(read_n_alleles(filename));
    /*
    ifstream fp(filename);
    char s[50],outputfilename[50];
    if(!fp) invalid_argument("Can't find parameter file.");

    std::vector<variable> variables;
    while(fp>>s)
    {
        //variables.push_back({s, })
        if(strcmp(s,"alleles")==0) { fp>>parameters.Nx>>parameters.Np>>parameters.Nq;}
        if(strcmp(s,"histbin")==0) { fp>>histbinx>>histbinp>>histbinq;}
        if(strcmp(s,"seed")==0) {fp>>parameters.seed;}
        if(strcmp(s,"pop0")==0) {fp>>parameters.popsize;}
        if(strcmp(s,"type0")==0){fp>>x0>>p0>>q0;}
        if(strcmp(s,"end")==0) {fp>>endtime;}
        if(strcmp(s,"sc")==0) {fp>>sc;}
        if(strcmp(s,"se")==0) {fp>>se;}
        if(strcmp(s,"sm")==0) {fp>>sm;}
        if(strcmp(s,"sv")==0) {fp>>sv;}
        if(strcmp(s,"sq")==0) {fp>>sq;}
        if(strcmp(s,"sk")==0) {fp>>sk;}
        if(strcmp(s,"c")==0) {fp>>c;}
        if(strcmp(s,"b")==0) {fp>>b;}
        if(strcmp(s,"eta")==0) {fp>>eta;}
        if(strcmp(s,"output")==0)
        {
            fp>>parameters.outputfreq>>outputfilename;
            out.open(outputfilename);
            if(!out) invalid_argument("Unable to open datafile.");
        }
    }
    fp.close();
  */
    return parameters;
}

/*
int main(int argc, char *argv[])
{
    if(argc==1) invalid_argument("no parameterfile specified");
    readparameters(argv[1]);
    initialize();

    vector<vector<double>> histX;
    vector<vector<double>> histP;
    vector<vector<double>> histQ;

    iterate(histX, histP, histQ);

    outputLTT(histX, histP, histQ);


  return 0;
}
*/
