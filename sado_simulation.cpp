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
#include "sado_output.h"

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
  const my_iterator i,
  const std::vector<double>& as
)
{
  const double sum_a{as.back() + eta};
  offspring kids;
  for(double nkid=0.0;;nkid+=1.0)
  {
    if(Uniform()>=b-nkid) break;
    const double draw=Uniform()*sum_a;
    if(draw>eta)
    {
      int index{0};
      for(auto j=std::cbegin(pop);j!=std::cend(pop);j++)
      {
        if (draw<=as[index] + eta)
        {
          assert(j != std::end(pop));
          assert(i != j); //Individual has attractiveness zero towards itself
          const indiv kid = create_offspring(*i, *j);
          kids.push_back(kid); //Kids are placed at the end of the population
          break;
        }
        ++index;
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

int sado::pick_random_individual_index(
  const int pop_size
)
{
  return std::floor(Uniform() * pop_size);
}


sado::my_iterator sado::randomindividual(
  population& pop,
  const int pop_size
)
{
  const int j{pick_random_individual_index(pop_size)};
  my_iterator that_one{std::begin(pop)};
  std::advance(that_one, j);
  return that_one;
}

sado::my_iterator sado::get_nth_individual(
  population& pop,
  const int n
)
{
  my_iterator that_one{std::begin(pop)};
  std::advance(that_one, n);
  return that_one;
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
      const int index{pick_random_individual_index(pop_size)};
      const auto i = get_nth_individual(pop, index);
      //const auto i = randomindividual(pop, pop_size);
      const double xi=i->get_x();
      const double pi=i->get_p();
      const double qi=i->get_q();
      const double comp{calc_comp(pop, xi)};
      if(Uniform()<(1.0-comp*c/gauss(xi,sk))*(0.5+0.5*gauss(qi,sq)))
      {
        const std::vector<double> as{get_summed_attractivenesses(pop, i, pi, xi)};
        const auto kids = create_kids(pop, i, as);
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



std::vector<double> sado::get_attractivenesses(
  const population& pop,
  const my_iterator i,
  const double pi,
  const double xi
)
{
  std::vector<double> as(pop.size(), 0.0);
  int index{0};
  for(auto j=std::cbegin(pop);j!=std::cend(pop);j++)
  {
    if(j!=i)
    {
      double qj=j->get_q();
      double xj=j->get_x();
      as[index] = gauss(pi-qj,sm)*gauss(xi-xj,se);
    }
    else
    {
      as[index] = 0.0;
    }
    ++index;
  }
  return as;
}

std::vector<double> sado::get_summed_attractivenesses(
  const population& pop,
  const my_iterator i,
  const double pi,
  const double xi
)
{
  return get_summed(
    get_attractivenesses(
      pop,
      i,
      pi,
      xi
    )
  );
}

