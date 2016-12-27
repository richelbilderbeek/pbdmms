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
#include "sado_random.h"
#include "sado_helper.h"

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
    [xi](double init, const indiv& i)  { return init + sado::gauss(xi - i.get_x(), sc); }
  );
}

sado::offspring sado::create_kids(
  const population& pop,
  const double sum_a,
  const my_iterator i
)
{
  offspring kids;
  for(double nkid=0.0;;nkid+=1.0)
  {
    if(Uniform()>=b-nkid) break;
    const double draw=Uniform()*sum_a;
    if(draw>eta)
    {
      for(auto j=std::cbegin(pop);j!=std::cend(pop);j++)
      {
        if(j!=i && draw<=j->get_a())
        {
          assert(j != std::end(pop));
          assert(i != j);
          const indiv kid = create_offspring(*i, *j);
          kids.push_back(kid); //Kids are placed at the end of the population
          //++pop_size;
          break;
        }
      }
    }
  }
  return kids;
}

void sado::do_simulation(const std::string& filename)
{
  const parameters p{
    readparameters(filename)
  };
  population pop = initialize(p);
  iterate(pop, p);
}

sado::my_iterator sado::randomindividual(
  population& pop,
  const int pop_size
)
{
  int k=0;

  const int j = int(floor(Uniform()* pop_size));

  for(auto i=std::begin(pop); i!=std::end(pop);i++,k++)
  {
    if(k==j)
    {
      return i;
    }
  }
  assert(!"Should not get here");
  return std::begin(pop);
}


sado::population sado::initialize(
  const parameters& p
)
{
  population pop;
  indiv eve;
  SetSeed(seed);
  eve.init(x0,p0,q0);
  pop.resize(p.get_pop_size(), eve);
  out<<"generation,popsize,rhoxp,rhoxq,rhopq,sx,sp,sq";
  for(int k=0;k<histw;k++) out<<","<<(k-histw/2)*histbinx;
  for(int k=0;k<histw;k++) out<<","<<(k-histw/2)*histbinp;
  for(int k=0;k<histw;k++) out<<","<<(k-histw/2)*histbinq;
  out<<endl;
  return pop;
}


void sado::output(
  const population& pop,
  int t, const int pop_size)
{
  double rhoxp,rhoxq,rhopq,
      ssxx=0.0,ssxp=0.0,sspp=0.0,ssxq=0.0,ssqq=0.0,sspq=0.0,dxi,dpi,dqi,
      maxx=0.0,maxp=0.0,maxq=0.0,sx,sp,sq,xi,pi,qi;
  int j,jx,jp,jq;

  const double delta{1.0/pop_size};
  std::vector<double> histx(histw, 0.0);
  std::vector<double> histp(histw, 0.0);
  std::vector<double> histq(histw, 0.0);
  //for(j=0;j<histw;j++)
  //{
  //  histx[j]=0.0;
  //  histp[j]=0.0;
  //  histq[j]=0.0;
  //}
  const double avgx{get_mean_x(pop)};
  const double avgp{get_mean_p(pop)};
  const double avgq{get_mean_q(pop)};
  for(auto i=std::cbegin(pop);i!=std::cend(pop);i++)
    {
      xi=i->get_x();
      pi=i->get_p();
      qi=i->get_q();
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
    append_histogram(histx, "eco_traits.csv");
    append_histogram(histp, "fem_prefs.csv");
    append_histogram(histq, "male_traits.csv");
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

void sado::iterate(population& pop, const parameters& p)
{
  for(int t=0;t<=endtime;++t)
  {
    int pop_size{static_cast<int>(pop.size())};
    if(pop.empty()) return;
    if(t%outputfreq==0)
    {
      output(pop, t, pop_size);
    }
    for(int k=0;k<pop_size;++k)
    {
      if(pop.empty())
      {
        return;
      }
      const auto i = randomindividual(pop, pop_size);
      const double xi=i->get_x();
      const double pi=i->get_p();
      const double qi=i->get_q();
      const double comp{calc_comp(pop, xi)};
      if(Uniform()<(1.0-comp*c/gauss(xi,sk))*(0.5+0.5*gauss(qi,sq)))
      {
        //sum_a: the sum of all attractivenesses
        std::vector<double> as(pop.size(), 0.0);
        const double sum_a{set_and_sum_attractivenesses(pop, i, pi, xi, as)};
        assert(sum_a == as.back());
        const auto kids = create_kids(pop, sum_a, i);
        for (auto kid: kids)
        {
          pop.push_back(kid);
          ++pop_size;
        }

      }
      if (p.get_erasure() == erasure::erase)
      {
        pop.erase(i);
      }
      else
      {
        std::swap(*i, pop.back());
        pop.pop_back();
      }
      --pop_size;
    }
  }
}

void sado::append_histogram(const std::vector<double>& p, const std::string& filename)
{
  assert(!p.empty());
  const double m{
    *std::max_element(
      std::begin(p),
      std::end(p)
    )
  };
  assert(m != 0.0);

  std::stringstream s;
  for (double d: p)
  {
    s << (d / m) << ',';
  }
  std::string t{s.str()};
  assert(!t.empty());
  t.resize(t.size() - 1);

  std::ofstream f(filename, std::ios_base::app);
  f << t << '\n';
}

double sado::set_and_sum_attractivenesses(
  population& pop,
  const my_iterator i,
  const double pi,
  const double xi,
  std::vector<double>& as
)
{
  //sum_a: sum of attractiveness
  double sum_a=eta;
  int index{0};
  for(auto j=std::begin(pop);j!=std::end(pop);j++)
  {
    if(j!=i)
    {
      double qj=j->get_q();
      double xj=j->get_x();
      sum_a+=gauss(pi-qj,sm)*gauss(xi-xj,se);
      j->set_a(sum_a);
    }
    as[index] = sum_a;
    ++index;
  }
  return sum_a;
}

