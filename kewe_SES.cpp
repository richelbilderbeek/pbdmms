#include "kewe_SES.h"
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
#include"kewe_random.h"
#include <cassert>
#include <vector>
#include <string>
#include "kewe_individual.h"
#include "kewe_globals.h"
#include "kewe_parameters.h"
simulation::simulation()
{

}

void simulation::run()
{
  const std::string filename("test_kewe_simulation.csv");
  create_test_parameter_file(filename);
  kewe_parameters parameters = readparameters(filename.c_str());
  std::list<indiv> pop = initialize();

  std::vector<std::vector<double>> histX;
  std::vector<std::vector<double>> histP;
  std::vector<std::vector<double>> histQ;

  iterate(histX, histP, histQ, parameters, pop);

  outputLTT(histX, histP, histQ, parameters);
}


using namespace std;



typedef list<indiv>::iterator my_iterator;

struct variable
{
    std::string name;
    double value1;
    double value2;
    double value3;
};

void create_test_parameter_file(const std::string& filename)
{
    std::ofstream f(filename.c_str());
    f << "STUB";
}


/*inline my_iterator std::begin(pop)
{ return pop.begin();}

inline my_iterator end(void)
{ return pop.std::end(pop);}*/

inline double gauss(double xx, double sigma)
{ return exp(-(xx*xx)/(2.0*sigma*sigma));}

// Pick random individual
my_iterator randomindividual(std::list<indiv>& pop)
{
  kewe_parameters parameters;
    bigint k=0;

    // Pick random number between 0 and populationsize
    const bigint j=bigint(floor(Uniform()*parameters.popsize));

    // return the random individual
    for(my_iterator i=std::begin(pop); i!=std::end(pop);i++,k++)
        if(k==j)
            return i;
    assert(!"Should not get here.");
}


std::list<indiv> initialize()
{
    kewe_parameters parameters; //Testing parameters by default
    const int histw = parameters.histw;
    std::ofstream out (parameters.outputfilename);
    std::list<indiv> pop;
    bigint j;
    int k;
    indiv I(parameters);

    SetSeed(parameters.seed);
    I.init(x0,p0,q0);

    for(j=0;j<parameters.popsize;j++)
        pop.push_back(I);

    out<<"generation,popsize,rhoxp,rhoxq,rhopq,sx,sp,sq,";

    for(k=0;k<histw;k++)
        out<<","<<(k-histw/2)*histbinx;
    for(k=0;k<histw;k++)
        out<<","<<(k-histw/2)*histbinp;
    for(k=0;k<histw;k++)
        out<<","<<(k-histw/2)*histbinq;
    out<<endl;
    return pop;
}


void output(bigint t,
            vector<vector<double>> &histX,
            vector<vector<double>> &histP,
            vector<vector<double>> &histQ,
            const kewe_parameters& parameters,
            const std::list<indiv>& pop
            )
{
  std::ofstream out(parameters.outputfilename);
  const int histw = parameters.histw;
  double avgp=0.0,avgq=0.0,avgx=0.0, rhoxp,rhoxq,rhopq,
      ssxx=0.0,ssxp=0.0,sspp=0.0,ssxq=0.0,ssqq=0.0,sspq=0.0,dxi,dpi,dqi,delta,
      maxx=0.0,maxp=0.0,maxq=0.0, sx,sp,sq,xi,pi,qi;
  my_iterator i;
  int j,jx,jp,jq;

  delta=1.0/parameters.popsize;

  std::vector<double> histx(histw, 0.0);
  std::vector<double> histp(histw, 0.0);
  std::vector<double> histq(histw, 0.0);

  for(auto i=std::begin(pop);i!=std::end(pop);i++)
  {
      avgx+=i->_x();
      avgp+=i->_p();
      avgq+=i->_q();

  }
  avgx/=parameters.popsize;
  avgp/=parameters.popsize;
  avgq/=parameters.popsize;

  for(auto i=std::begin(pop);i!=std::end(pop);i++)
  {
      xi=i->_x();
      pi=i->_p();
      qi=i->_q();

      dxi=xi-avgx;
      dpi=pi-avgp;
      dqi=qi-avgq;
      ssxx+=dxi*dxi;
      ssxp+=dxi*dpi;
      ssxq+=dxi*dqi;
      sspp+=dpi*dpi;
      sspq+=dpi*dqi;
      ssqq+=dqi*dqi;

      jx=int(histw/2.0+xi/histbinx);
      jp=int(histw/2.0+pi/histbinp);
      jq=int(histw/2.0+qi/histbinq);

      if(jx<0) jx=0;
      if(jx>=histw) jx=histw-1;
      if(jp<0) jp=0;
      if(jp>=histw) jp=histw-1;
      if(jq<0) jq=0;
      if(jq>=histw) jq=histw-1;
      histx[jx]+=delta;
      if(histx[jx]>maxx) maxx=histx[jx];
      histp[jp]+=delta;
      if(histp[jp]>maxp) maxp=histp[jp];
      histq[jq]+=delta;
      if(histq[jq]>maxq) maxq=histq[jq];

    }
    rhoxp=ssxp/sqrt(ssxx*sspp);
    rhoxq=ssxq/sqrt(ssxx*ssqq);
    rhopq=sspq/sqrt(sspp*ssqq);
    sx=sqrt(ssxx/(parameters.popsize-1.0));
    sp=sqrt(sspp/(parameters.popsize-1.0));
    sq=sqrt(ssqq/(parameters.popsize-1.0));
    out<<t<<","<<parameters.popsize<<","<<rhoxp<<","<<rhoxq<<","<<rhopq<<","<<sx<<","<<sp<<","<<sq;
    cout<<t<<" "<<parameters.popsize<<" "<<rhoxp<<" "<<rhoxq<<" "<<rhopq<<endl
        <<avgx<<" "<<avgp<<" "<<avgq<<" "<<sx<<" "<<sp<<" "<<sq<<endl;

    vector<double> histXGen;
    vector<double> histPGen;
    vector<double> histQGen;


    assert(histXGen.empty());
    assert(histPGen.empty());
    assert(histQGen.empty());

    for(j=0;j<histw;j++)
    {
        out<<","<<histx[j]/maxx;
        histXGen.push_back(histx[j]/maxx);
    }

    for(j=0;j<histw;j++)
    {
        out<<","<<histp[j]/maxp;
        histPGen.push_back(histp[j]/maxp);
    }

    for(j=0;j<histw;j++)
    {
        out<<","<<histq[j]/maxq;
        histQGen.push_back(histq[j]/maxq);
    }
    histX.push_back(histXGen);
    histP.push_back(histPGen);
    histQ.push_back(histQGen);

    histXGen.clear();
    histPGen.clear();
    histQGen.clear();

    out<<endl;
    return;
}

void iterate(
  vector<vector<double>> &histX,
  vector<vector<double>> &histP,
  vector<vector<double>> &histQ,
  const kewe_parameters& parameters,
  std::list<indiv>& pop
)
{
    bigint popsize = parameters.popsize;
    my_iterator i,j;    // iterates through a vector/list (Keeps track of the individual
    indiv kid(parameters);  // potential baby
    bigint k,t;
    double nkid,comp,xi,pi,qi,xj,qj,attractiveness,draw;

    for(t=0;t<=endtime && popsize != 0;t++)
    {
        if(t%parameters.outputfreq==0) output(t, histX, histP, histQ, parameters, pop); // Output once every outputfreq

        for(k=0;k<popsize && popsize != 0;k++)
        {
            // Pick random individual and get its x, p and q loci
            const auto i = randomindividual(pop);
            xi=i->_x();
            pi=i->_p();
            qi=i->_q();

            comp=0.0; // competition

            for(j=std::begin(pop);j!=std::end(pop);j++) // Go through all individuals
            {
                if(j != i)
                {
                    xj= j -> _x();          // As long as J is not I, make xj j's x.
                    comp+=gauss(xi-xj,sc);  // Add intensity competition
                }
            }

            // If individual survives calculate its attractiveness
            if(Uniform() < (1.0 - comp * c / gauss(xi,sk)) * (0.5+0.5*gauss(qi,sq)))
            // 1.0 - comp ... ...sq))) == survival rate
            {
                attractiveness=eta;
                for(j=std::begin(pop);j!=std::end(pop);j++)
                {
                    if(j!=i)
                    {
                        qj=j->_q();
                        xj=j->_x();
                        attractiveness+=gauss(pi-qj,sm)*gauss(xi-xj,se);
                        // gauss ... ... se) == A ik --> attractivenes == formula[2] under the devision line.
                        j->a_(attractiveness); //set j's a to attractiveness.
                    }
                }

                for(nkid=0.0;;nkid+=1.0)
                {
                    if(Uniform()>=b-nkid) break; // have max b kids

                    draw=Uniform()*attractiveness;
                    if(draw>eta) // can female find an attractive male?
                    {
                        for(j=std::begin(pop);j!=std::end(pop);j++) // Go through all individuals
                        {
                            if(j!=i && draw<=j->_a()) // if male is attractive enough
                            {
                                kid.birth((*i),(*j), parameters); // i and j make baby
                                pop.push_back(kid); // add kid to population
                                popsize++;
                                break; // stop looking for mate
                            }
                        }
                    }
                }
            }
            // Then kill the individual (females die after mating)
            pop.erase(i);
            popsize--;
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

// Count number of borders (from 0 to >0 or from >0 to 0) in a histogram
int countBorders(const vector<double> &histogram)
{
    if (histogram.empty()) throw std::invalid_argument("Histogram is empty");

    int size = static_cast<int>(histogram.size());
    int numOfBorders{0};
    for (int i = 0; i<size; ++i)
    {
        double l, r, o = histogram[i];
        if (i==0) l = 0.0;
        else l = histogram[i-1];
        if (i==size-1) r = 0.0;
        else r = histogram[i+1];

        if ((i==0 && r >= 0.05) || (i==size-1 && l >=0.05)) o = 0.0;
        if (l >= 0.05 && o < 0.05 && r < 0.05) ++numOfBorders;
        if (l < 0.05 && o < 0.05 && r >= 0.05) ++numOfBorders;
    }

    return numOfBorders;
}

// calculates lineages (borders / 2) and the trait with the most lineages becomes
// the number of lineages for that generation
int countLineagesForGen(const int t,
                        const vector<vector<double>> &histX,
                        const vector<vector<double>> &histP,
                        const vector<vector<double>> &histQ)
{
    if (t < 0) throw std::invalid_argument("Time can't be negative");
    if (histX.empty()) throw std::invalid_argument("HistX is empty");
    if (histP.empty()) throw std::invalid_argument("HistP is empty");
    if (histQ.empty()) throw std::invalid_argument("HistQ is empty");

    int xBorders = countBorders(histX[t]);
    int pBorders = countBorders(histP[t]);
    int maxBorders = countBorders(histQ[t]);
    if (xBorders > maxBorders) maxBorders = xBorders;
    if (countBorders(histQ[t]) > maxBorders) maxBorders = pBorders;
    return maxBorders / 2;
}

//output all number of lineages for all the generations
void outputLTT(const vector<vector<double>> &histX,
               const vector<vector<double>> &histP,
               const vector<vector<double>> &histQ,
               const kewe_parameters& parameters )
{

    if (histX.empty()) throw std::invalid_argument("HistX is empty");
    if (histP.empty()) throw std::invalid_argument("HistP is empty");
    if (histQ.empty()) throw std::invalid_argument("HistQ is empty");

    ofstream LTT("ltt.csv");

    for (int i = 0; i < static_cast<int>(histX.size()); ++i)
        LTT << i * parameters.outputfreq << "," << countLineagesForGen(i, histX, histP, histQ) << '\n';
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
