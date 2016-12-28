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
#include "sado_population.h"
#include "sado_random.h"
#include "sado_helper.h"
#include "sado_output.h"

double sado::calc_comp(
  const population& pop,
  const double xi,
  const parameters& p
) noexcept
{
  return std::accumulate(
    std::begin(pop),
    std::end(pop),
    -1.0,
    [p, xi](double init, const indiv& i)
    {
      return init + sado::gauss(xi - i.get_x(), p.get_sc());
    }
  );
}

sado::offspring sado::create_kids(
  const population& pop,
  const indiv& mother,
  const std::vector<double>& raw_as,
  const parameters& p
)
{
  //Cumulative attractivenesses
  const double b{p.get_b()};
  const std::vector<double> as{get_summed(raw_as)};
  const double eta{p.get_eta()};
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
          const indiv kid = create_offspring(mother, pop[index], p);
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

  eve.init(p.get_x0(),p.get_p0(),p.get_q0(), p);
  pop.resize(p.get_pop_size(), eve);
  return pop;
}




void sado::iterate(population pop, const parameters& p)
{
  for(int t=0;t<=p.get_end_time();++t)
  {
    if(pop.empty()) return;
    if( t % p.get_output_freq()==0)
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
      const auto kids = try_to_create_kids(pop, index, p);
      for (auto kid: kids)
      {
        pop.push_back(kid);
      }
      kill_mother(index, pop, p);
    }
  }
}

void sado::kill_mother(
  const int index,
  population& pop,
  const parameters& p
)
{
  assert(index < static_cast<int>(pop.size()));
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

sado::offspring sado::try_to_create_kids(
  const population& pop,
  const int index,
  const parameters& p
)
{
  assert(index < static_cast<int>(pop.size()));
  const indiv mother{pop[index]};
  const double xi=mother.get_x();
  const double pi=mother.get_p();
  const double qi=mother.get_q();
  const double comp{calc_comp(pop, xi, p)};
  const double c{p.get_c()};
  const double sk{p.get_sk()};
  const double sq{p.get_sq()};
  if(Uniform()<(1.0-((comp*c)/gauss(xi,sk)))*(0.5+(0.5*gauss(qi,sq))))
  {
    //The attractivenesses you have with pi and xi
    std::vector<double> as{get_attractivenesses(pop, pi, xi, p)};
    //Unattracted to yourself
    as[index] = 0.0;
    //Get kids
    return create_kids(pop, mother, as, p);
  }
  return {}; //No kids
}

std::vector<double> sado::get_attractivenesses(
  const population& pop,
  const double pi,
  const double xi,
  const parameters& p
)
{
  std::vector<double> as(pop.size(), 0.0);
  int index{0};
  for(auto j=std::cbegin(pop);j!=std::cend(pop);j++)
  {
    const double qj{j->get_q()};
    const double xj{j->get_x()};
    const double se{p.get_se()};
    const double sm{p.get_sm()};
    as[index] = gauss(pi-qj,sm)*gauss(xi-xj,se);
    ++index;
  }
  return as;
}

