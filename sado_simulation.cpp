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
  const indiv& mother,
  const std::vector<double>& raw_as
)
{
  //Cumulative attractivenesses
  const std::vector<double> as{get_summed(raw_as)};
  const double sum_a{as.back() + eta};
  offspring kids;
  for(double nkid=0.0;;nkid+=1.0)
  {
    if(Uniform()>=b-nkid) break;
    const double draw=Uniform()*sum_a;
    if(draw>eta)
    {
      for(int index{0}; ; ++index)
      {
        //There must be an individual that is attractive enough
        assert(index < static_cast<int>(pop.size()));
        if (draw<=as[index] + eta)
        {
          const indiv kid = create_offspring(mother, pop[index]);
          kids.push_back(kid);
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

  SetSeed(p.get_seed());
  create_header(p);

  const population pop = create_initial_population(p);
  iterate(pop, p);
}

int sado::pick_random_individual_index(
  const int pop_size
)
{
  return std::floor(Uniform() * pop_size);
}

sado::population sado::create_initial_population(
  const parameters& p
)
{
  population pop;
  indiv eve;

  eve.init(p.get_x0(),p.get_p0(),p.get_q0());
  pop.resize(p.get_pop_size(), eve);
  return pop;
}




void sado::iterate(population pop, const parameters& p)
{
  for(int t=0;t<=endtime;++t)
  {
    if(pop.empty()) return;
    if(t%outputfreq==0)
    {
      output(pop, t, p);
    }
    for(int k=0;k<static_cast<int>(pop.size());++k)
    {
      if(pop.empty())
      {
        return;
      }
      const int index{pick_random_individual_index(pop.size())};
      const indiv mother{pop[index]};
      const double xi=mother.get_x();
      const double pi=mother.get_p();
      const double qi=mother.get_q();
      const double comp{calc_comp(pop, xi)};
      if(Uniform()<(1.0-((comp*c)/gauss(xi,sk)))*(0.5+(0.5*gauss(qi,sq))))
      {
        //The attractivenesses you have with pi and xi
        std::vector<double> as{get_attractivenesses(pop, pi, xi)};
        //Unattracted to yourself
        as[index] = 0.0;
        //Get kids
        const auto kids = create_kids(pop, mother, as);
        for (auto kid: kids)
        {
          pop.push_back(kid);
        }

      }
      if (p.get_erasure() == erasure_method::erase)
      {
        assert(index < static_cast<int>(pop.size()));
        pop.erase(std::begin(pop) + index);
      }
      else
      {
        assert(index < static_cast<int>(pop.size()));
        std::swap(pop[index], pop.back());
        pop.pop_back();
      }
    }
  }
}



std::vector<double> sado::get_attractivenesses(
  const population& pop,
  const double pi,
  const double xi
)
{
  std::vector<double> as(pop.size(), 0.0);
  int index{0};
  for(auto j=std::cbegin(pop);j!=std::cend(pop);j++)
  {
    double qj=j->get_q();
    double xj=j->get_x();
    as[index] = gauss(pi-qj,sm)*gauss(xi-xj,se);
    ++index;
  }
  return as;
}

