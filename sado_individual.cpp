#include "sado_individual.h"

#include <iostream>

sado::indiv::indiv()
{
  X.resize(1, 0.0);
  P.resize(1, 0.0);
  Q.resize(1, 0.0);
  x=0.0;
  p=0.0;
  q=0.0;
  a=0.0;
}

void sado::indiv::birth(const indiv& m, const indiv& f)
{
  //Note that genotype == phenotype (in this haploid case)
  X[0] = (Uniform() < 0.5 ? m.X[0] : f.X[0]) + Normal(0.0, sv);
  P[0] = (Uniform() < 0.5 ? m.P[0] : f.P[0]) + Normal(0.0, sv);
  Q[0] = (Uniform() < 0.5 ? m.Q[0] : f.Q[0]) + Normal(0.0, sv);
  x=X[0];
  p=P[0];
  q=Q[0];
}

sado::indiv sado::create_offspring(const indiv& m, const indiv& f)
{
  indiv kid;
  kid.birth(m, f);
  return kid;
}

void sado::indiv::init(double this_x0, double this_p0, double this_q0)
{
  for(int i=0;i<1;i++) X[i]=this_x0+Normal(0.0,sv);
  for(int i=0;i<1;i++) P[i]=this_p0+Normal(0.0,sv);
  for(int i=0;i<1;i++) Q[i]=this_q0+Normal(0.0,sv);
  x=this_x0+Normal(0.0,sv);
  p=this_p0+Normal(0.0,sv);
  q=this_q0+Normal(0.0,sv);
}

void sado::indiv::print() const noexcept
{
  std::cout << x << " " << p << " " << q << '\n';
  for(int i=0;i<1;i++) std::cout<<X[i]<<" ";
  std::cout<<'\n';
  for(int i=0;i<1;i++) std::cout<<P[i]<<" ";
  std::cout<<'\n';
  for(int i=0;i<1;i++) std::cout<<Q[i]<<" ";
  std::cout<<'\n';
}

bool sado::operator==(const indiv& lhs, const indiv& rhs) noexcept
{
  return lhs._x() == rhs._x()
    && lhs._p() == rhs._p()
    && lhs._q() == rhs._q()
  ;
}
