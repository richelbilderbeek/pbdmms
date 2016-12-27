#include "sado_simulation.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <numeric>
#include <cstring>
#include <iostream>
#include <sstream>
#include <boost/algorithm/string/split.hpp>
#include "sado_individual.h"
#include "sado_globals.h"
#include "sado_population.h"

using namespace std;

double sado::calc_comp(
  const population& p,
  const double xi
) noexcept
{
  return std::accumulate(
    std::begin(p),
    std::end(p),
    -1.0,
    [xi](double init, const indiv& i)  { return init + gauss(xi - i._x(), sc); }
  );
}

void sado::create_kids(
  const double attractiveness,
  const my_iterator i,
  int& pop_size
)
{
  for(double nkid=0.0;;nkid+=1.0)
  {
    if(Uniform()>=b-nkid) break;
    const double draw=Uniform()*attractiveness;
    if(draw>eta)
    {
      for(my_iterator j=std::begin(pop);j!=std::end(pop);j++)
      {
        if(j!=i && draw<=j->_a())
        {
          //assert(i >= pop.begin());
          //assert(i < pop.std::end(pop));
          //assert(j >= pop.begin());
          assert(j != std::end(pop));
          assert(i != j);
          indiv kid;
          kid.birth(*i,*j);
          pop.push_back(kid);
          ++pop_size;
          break;
        }
      }
    }
  }
}

void sado::do_simulation(const std::string& filename)
{
  const parameters p{
    readparameters(filename)
  };
  initialize(p);
  iterate();
}

bool sado::is_more_or_less_same(
  const std::vector<double>& v,
  const std::vector<double>& w
)
{
  assert(v.size() == w.size());
  const int sz{static_cast<int>(v.size())};
  for (int i=0; i!=sz; ++i)
  {
    if (std::abs(v[i] - w[i]) > 0.0001) return false;
  }
  return true;
}

double sado::gauss(double xx, double sigma)
{
  return exp(-(xx*xx)/(2.0*sigma*sigma));
}

sado::my_iterator sado::randomindividual(const int pop_size)
{
  bigint k=0;

  const bigint j = bigint(floor(Uniform()* pop_size));

  for(my_iterator i=std::begin(pop); i!=std::end(pop);i++,k++)
  {
    if(k==j)
    {
      return i;
    }
  }
  assert(!"Should not get here");
  return std::begin(pop);
}


void sado::initialize(const parameters& p)
{
  indiv eve;
  SetSeed(seed);
  eve.init(x0,p0,q0);
  for(int j=0;j!=p.m_pop_size;j++) pop.push_back(eve);
  out<<"generation,popsize,rhoxp,rhoxq,rhopq,sx,sp,sq";
  for(int k=0;k<histw;k++) out<<","<<(k-histw/2)*histbinx;
  for(int k=0;k<histw;k++) out<<","<<(k-histw/2)*histbinp;
  for(int k=0;k<histw;k++) out<<","<<(k-histw/2)*histbinq;
  out<<endl;
}


void sado::output(bigint t, const int pop_size)
{
  double avgp=0.0,avgq=0.0,avgx=0.0,rhoxp,rhoxq,rhopq,
      ssxx=0.0,ssxp=0.0,sspp=0.0,ssxq=0.0,ssqq=0.0,sspq=0.0,dxi,dpi,dqi,delta,
      maxx=0.0,maxp=0.0,maxq=0.0,sx,sp,sq,xi,pi,qi;
  my_iterator i;
  int j,jx,jp,jq;

  delta=1.0/pop_size;
  for(j=0;j<histw;j++)
    {
      histx[j]=0.0;
      histp[j]=0.0;
      histq[j]=0.0;
    }

  for(i=std::begin(pop);i!=std::end(pop);i++)
    {
      avgx+=i->_x();
      avgp+=i->_p();
      avgq+=i->_q();
    }
  avgx/=pop_size;
  avgp/=pop_size;
  avgq/=pop_size;
  for(i=std::begin(pop);i!=std::end(pop);i++)
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
  sx=sqrt(ssxx/(pop_size-1.0));
  sp=sqrt(sspp/(pop_size-1.0));
  sq=sqrt(ssqq/(pop_size-1.0));

  std::stringstream s;
  s  <<t<<","<<pop_size<<","<<rhoxp<<","<<rhoxq<<","<<rhopq<<","<<sx<<","<<sp<<","<<sq;
  out<<t<<","<<pop_size<<","<<rhoxp<<","<<rhoxq<<","<<rhopq<<","<<sx<<","<<sp<<","<<sq;
  cout<<t<<" "<<pop_size<<" "<<rhoxp<<" "<<rhoxq<<" "<<rhopq<<endl
     <<avgx<<" "<<avgp<<" "<<avgq<<" "<<sx<<" "<<sp<<" "<<sq<<endl;

  {
    append_histogram(histx, histw, "eco_traits.csv");
    append_histogram(histp, histw, "fem_prefs.csv");
    append_histogram(histq, histw, "male_traits.csv");
  }
  for(j=0;j<histw;j++)
  {
    out<<","<<histx[j]/maxx;
    s  <<","<<histx[j]/maxx;
  }
  for(j=0;j<histw;j++)
  {
    out<<","<<histp[j]/maxp;
    s  <<","<<histp[j]/maxp;
  }
  for(j=0;j<histw;j++)
  {
    out<<","<<histq[j]/maxq;
    s  <<","<<histq[j]/maxq;
  }
  out<<endl;
  const std::string golden{get_golden_output().at( (t / 10) + 1)};
  const std::string measured{s.str()};
  const std::vector<double> golden_values{
    to_doubles(seperate_string(golden, ','))
  };
  const std::vector<double> measured_values{
    to_doubles(seperate_string(measured, ','))
  };
  std::clog << "Comparing:\n"
    << "golden  : " << golden << '\n'
    << "measured: " << measured << '\n'
  ;
  assert(is_more_or_less_same(golden_values, measured_values));
}



std::vector<std::string> sado::get_golden_output() noexcept
{
  //Created with 'cat output.txt | xclip -selection c'
  return {
    "generation,popsize,rhoxp,rhoxq,rhopq,sx,sp,sq,-2.5,-2.4,-2.3,-2.2,-2.1,-2,-1.9,-1.8,-1.7,-1.6,-1.5,-1.4,-1.3,-1.2,-1.1,-1,-0.9,-0.8,-0.7,-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2,2.1,2.2,2.3,2.4,-2.5,-2.4,-2.3,-2.2,-2.1,-2,-1.9,-1.8,-1.7,-1.6,-1.5,-1.4,-1.3,-1.2,-1.1,-1,-0.9,-0.8,-0.7,-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2,2.1,2.2,2.3,2.4,-2.5,-2.4,-2.3,-2.2,-2.1,-2,-1.9,-1.8,-1.7,-1.6,-1.5,-1.4,-1.3,-1.2,-1.1,-1,-0.9,-0.8,-0.7,-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2,2.1,2.2,2.3,2.4",
    "0,1000,1,1,1,4.38758e-15,9.88593e-15,7.33114e-15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0",
    "10,1329,0.028831,-0.0250136,0.0378533,0.0658857,0.0599517,0.0589463,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.00153846,0.173846,1,0.78,0.0784615,0.00923077,0.00153846,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.00651466,0.156352,1,0.928339,0.0716612,0.00162866,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.0307076,0.59012,1,0.146862,0.00667557,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0",
    "20,1562,-0.0325528,-0.00643697,0.0361947,0.155312,0.0890938,0.0699922,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.0018622,0.0465549,0.327747,0.528864,1,0.631285,0.178771,0.0297952,0.0782123,0.0744879,0.0111732,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.0674536,0.367622,0.913997,1,0.266442,0.0185497,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.145015,0.987915,1,0.20997,0.0166163,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0",
    "30,2007,-0.09337,-0.0425089,0.129271,0.319202,0.107503,0.0809984,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.00280112,0.145658,0.714286,0.910364,1,0.621849,0.330532,0.263305,0.10084,0.268908,0.641457,0.495798,0.123249,0.00280112,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.0063593,0.127186,0.63593,1,0.945946,0.422893,0.0524642,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.0184729,0.285714,1,0.935961,0.213054,0.0184729,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0",
    "40,2287,-0.0284915,0.0131021,0.0497291,0.402244,0.119784,0.0898951,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.0143541,0.229665,0.925837,1,0.624402,0.296651,0.186603,0.126794,0.110048,0.196172,0.425837,0.5,0.526316,0.289474,0.0191388,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.0190616,0.209677,0.708211,1,0.900293,0.410557,0.0909091,0.0146628,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.00114679,0.00114679,0.0447248,0.348624,1,0.930046,0.276376,0.0206422,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0",
    "50,2502,0.0346634,0.031365,0.123172,0.454428,0.145735,0.0863978,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.00909091,0.0522727,0.186364,0.665909,1,0.8,0.456818,0.215909,0.120455,0.0454545,0.104545,0.313636,0.3,0.338636,0.475,0.406818,0.181818,0.0136364,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.00420168,0.0854342,0.247899,0.686275,1,0.782913,0.42437,0.190476,0.0714286,0.0112045,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.0627273,0.354545,1,0.682727,0.167273,0.00727273,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0",
    "60,2699,-0.129535,-0.0689655,0.129737,0.502419,0.155007,0.086267,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.0326087,0.586957,0.884058,1,0.858696,0.855072,0.869565,0.608696,0.322464,0.253623,0.278986,0.376812,0.380435,0.59058,0.460145,0.630435,0.460145,0.155797,0.0471014,0.112319,0.0144928,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.00256739,0.0397946,0.0667522,0.191271,0.573813,1,0.840822,0.408216,0.231065,0.0706033,0.0333761,0.00641849,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.00271493,0.0307692,0.388235,1,0.806335,0.2,0.0144796,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0",
    "70,3017,-0.011253,-0.0906589,0.128882,0.613459,0.160398,0.0892942,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.00634921,0.0698413,0.644444,1,0.911111,0.571429,0.409524,0.384127,0.571429,0.511111,0.257143,0.263492,0.311111,0.546032,0.390476,0.593651,0.285714,0.428571,0.393651,0.219048,0.0952381,0.4,0.301587,0.0126984,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.00501882,0.02133,0.0652447,0.248432,0.608532,1,0.962359,0.450439,0.250941,0.125471,0.033877,0.00501882,0.00878294,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.00262238,0.0559441,0.394231,1,0.943182,0.22028,0.020979,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0",
    "80,3248,-0.0146235,-0.0288981,0.120524,0.66588,0.167731,0.0927048,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.0597403,0.605195,1,0.631169,0.425974,0.257143,0.220779,0.350649,0.4,0.425974,0.353247,0.306494,0.366234,0.581818,0.290909,0.301299,0.244156,0.337662,0.132468,0.166234,0.220779,0.290909,0.34026,0.127273,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.00122549,0.0453431,0.156863,0.286765,0.588235,1,0.92402,0.595588,0.25,0.0784314,0.0453431,0.00490196,0.00367647,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.000812348,0.0519903,0.393989,1,0.909829,0.251015,0.0259951,0.00487409,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0",
    "90,3399,-0.00225078,0.0502589,0.19751,0.692821,0.159965,0.096373,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.00641026,0.0833333,0.605769,1,0.923077,0.637821,0.442308,0.214744,0.221154,0.548077,0.589744,0.554487,0.567308,0.330128,0.477564,0.730769,0.294872,0.291667,0.346154,0.339744,0.285256,0.144231,0.25641,0.349359,0.307692,0.24359,0.102564,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.00928074,0.0348028,0.175174,0.262181,0.719258,0.828306,1,0.635731,0.222738,0.0336427,0.0139211,0.00464037,0.00348028,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.0071599,0.0795545,0.5179,1,0.821002,0.256165,0.0222753,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0",
    "100,3463,-0.000125474,-0.00751536,0.194455,0.737729,0.168195,0.092114,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.0035461,0.0815603,0.425532,1,0.911348,0.758865,0.695035,0.27305,0.212766,0.202128,0.70922,0.787234,0.546099,0.542553,0.22695,0.702128,0.712766,0.414894,0.258865,0.468085,0.265957,0.390071,0.191489,0.198582,0.464539,0.262411,0.156028,0.141844,0.234043,0.0425532,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.0135318,0.124493,0.461434,0.843031,0.807848,1,0.939107,0.396482,0.0649526,0.00405954,0.0189445,0.0108254,0.00135318,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.000761035,0.0418569,0.420852,1,0.891933,0.243531,0.0327245,0.00380518,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0",
  };
}

void sado::iterate()
{
  for(int t=0;t<=endtime;++t)
  {
    int pop_size{static_cast<int>(pop.size())};
    if(pop.empty()) return;
    if(t%outputfreq==0)
    {
      output(t, pop_size);
    }
    for(int k=0;k<pop_size;++k)
    {
      if(pop.empty())
      {
        return;
      }
      const my_iterator i = randomindividual(pop_size);
      const double xi=i->_x();
      const double pi=i->_p();
      const double qi=i->_q();
      const double comp{calc_comp(pop, xi)};
      if(Uniform()<(1.0-comp*c/gauss(xi,sk))*(0.5+0.5*gauss(qi,sq)))
      {
        const double attractiveness{set_and_sum_attractivenesses(i, pi, xi)};
        create_kids(attractiveness, i, pop_size);
      }
      //#define SADO_USE_SWAP_TRICK
      #ifndef SADO_USE_SWAP_TRICK
      pop.erase(i);
      #else
      std::swap(*i, pop.back());
      pop.pop_back();
      #endif
      --pop_size;
    }
  }
}

sado::parameters sado::readparameters(const std::string& filename)
{
  parameters p;
  ifstream fp(filename);
  char s[50],outputfilename[50];
  cout<<"reading parameters and initializing"<<endl;
  if(!fp) exit(1);
  cout<<"opening parameterfile"<<endl;
  while(fp>>s)
    {
      if(strcmp(s,"alleles")==0) { fp>>Nx>>Np>>Nq; cout<<"parameters "<<s<<" set to "<<Nx<<" "<<Np<<" "<<Nq<<endl;}
      if(strcmp(s,"histbin")==0) { fp>>histbinx>>histbinp>>histbinq; cout<<"parameters "<<s<<" set to "<<histbinx<<" "<<histbinp<<" "<<histbinq<<endl;}
      if(strcmp(s,"seed")==0) {fp>>seed; cout<<"parameter "<<s<<" set to "<<seed<<endl;}
      if(strcmp(s,"pop0")==0) { fp >> p.m_pop_size; cout <<"parameter "<< s << " set to "<< p.m_pop_size << '\n'; }
      if(strcmp(s,"type0")==0)
        {
          fp>>x0>>p0>>q0;
          cout<<"parameter x0 set to "<<x0<<endl;
          cout<<"parameter p0 set to "<<p0<<endl;
          cout<<"parameter q0 set to "<<q0<<endl;
        }
      if(strcmp(s,"end")==0) {fp>>endtime;cout<<"parameter "<<s<<" set to "<<endtime<<endl;}
      if(strcmp(s,"sc")==0) {fp>>sc;cout<<"parameter "<<s<<" set to "<<sc<<endl;}
      if(strcmp(s,"se")==0) {fp>>se;cout<<"parameter "<<s<<" set to "<<se<<endl;}
      if(strcmp(s,"sm")==0) {fp>>sm;cout<<"parameter "<<s<<" set to "<<sm<<endl;}
      if(strcmp(s,"sv")==0) {fp>>sv;cout<<"parameter "<<s<<" set to "<<sv<<endl;}
      if(strcmp(s,"sq")==0) {fp>>sq;cout<<"parameter "<<s<<" set to "<<sq<<endl;}
      if(strcmp(s,"sk")==0) {fp>>sk;cout<<"parameter "<<s<<" set to "<<sk<<endl;}
      if(strcmp(s,"c")==0) {fp>>c;cout<<"parameter "<<s<<" set to "<<c<<endl;}
      if(strcmp(s,"b")==0) {fp>>b;cout<<"parameter "<<s<<" set to "<<b<<endl;}
      if(strcmp(s,"eta")==0) {fp>>eta;cout<<"parameter "<<s<<" set to "<<eta<<endl;}
      if(strcmp(s,"output")==0)
        {
          fp>>outputfreq>>outputfilename;
          cout<<"saving data every "<<outputfreq<<" generations in "<<outputfilename<<endl;
          out.open(outputfilename);
          if(!out) {cout<<"unable to open datafile"<<endl; exit(1);}
        }
      if(strcmp(s,"haploid")==0)
        {
          haploid=1;
          diploid=0;
          cout<<"haploid genetic system"<<endl;
        }
      if(strcmp(s,"diploid")==0)
        {
          haploid=0;
          diploid=1;
          cout<<"diploid genetic system"<<endl;
        }
    }
  fp.close();
  return p;
}

void sado::append_histogram(const double * const p, const int sz, const std::string& filename)
{
  const double m{
    *std::max_element(p, p + sz)
  };

  std::stringstream s;
  for (int i=0; i!=sz; ++i)
  {
    s << (p[i] / m) << ',';
  }
  std::string t{s.str()};
  assert(!t.empty());
  t.resize(t.size() - 1);

  std::ofstream f(filename, std::ios_base::app);
  f << t << '\n';
}

std::vector<std::string> sado::seperate_string(
  const std::string& input,
  const char seperator
)
{
  std::vector<std::string> v;
  boost::algorithm::split(v,input,
  std::bind2nd(std::equal_to<char>(),seperator),
  boost::algorithm::token_compress_on);
  return v;
}

double sado::set_and_sum_attractivenesses(
  const my_iterator i,
  const double pi,
  const double xi
)
{
  double attractiveness=eta;
  for(my_iterator j=std::begin(pop);j!=std::end(pop);j++)
  {
    if(j!=i)
    {
      double qj=j->_q();
      double xj=j->_x();
      attractiveness+=gauss(pi-qj,sm)*gauss(xi-xj,se);
      j->a_(attractiveness);
    }
  }
  return attractiveness;
}

std::vector<double> sado::to_doubles(
  const std::vector<std::string>& v
)
{
  std::vector<double> w;
  w.reserve(v.size());
  std::transform(
    std::begin(v),
    std::end(v),
    std::back_inserter(w),
    [](const std::string& s) { return std::stod(s); }
  );
  return w;
}
