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
      int index{0};
      for(auto j=std::cbegin(pop);j!=std::cend(pop);j++)
      {
        if (draw<=as[index] + eta)
        {
          assert(j != std::end(pop));
          const indiv kid = create_offspring(mother, *j);
          kids.push_back(kid);
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

  SetSeed(seed);
  create_header();

  const population pop = create_initial_population(p);
  iterate(pop, p);
}

int sado::pick_random_individual_index(
  const int pop_size
)
{
  return std::floor(Uniform() * pop_size);
}


std::vector<sado::indiv>::iterator sado::find_nth_individual(
  std::vector<indiv>& pop,
  const int n
)
{
  return std::begin(pop) + n;
}

std::list<sado::indiv>::iterator sado::find_nth_individual(
  std::list<sado::indiv>& pop,
  const int n
)
{
  assert(n < static_cast<int>(pop.size()));
  auto that_one{std::begin(pop)};
  std::advance(that_one, n);
  return that_one;
}

sado::indiv sado::get_nth_individual(
  const population& pop,
  const int n
)
{
  assert(n >= 0);
  assert(n < static_cast<int>(pop.size()));
  auto that_one = std::cbegin(pop);
  std::advance(that_one, n);
  return *that_one;
}


sado::population sado::create_initial_population(
  const parameters& p
)
{
  population pop;
  indiv eve;
  eve.init(x0,p0,q0);
  pop.resize(p.get_pop_size(), eve);
  return pop;
}




void sado::iterate(population pop, const parameters& p)
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
      const indiv mother{get_nth_individual(pop, index)};
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
          ++pop_size;
        }

      }
      if (p.get_erasure() == erasure::erase)
      {
        const auto i = find_nth_individual(pop, index);
        pop.erase(i);
      }
      else
      {
        const auto i = find_nth_individual(pop, index);
        std::swap(*i, pop.back());
        pop.pop_back();
      }
      --pop_size;
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

