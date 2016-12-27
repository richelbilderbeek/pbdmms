#include "sado_individual.h"

#include <iostream>

sado::indiv::indiv()
{
  m_x_gen.resize(1, 0.0);
  m_p_gen.resize(1, 0.0);
  m_q_gen.resize(1, 0.0);
  x=0.0;
  p=0.0;
  q=0.0;
  a=0.0;
}

void sado::indiv::birth(const indiv& m, const indiv& f)
{
  //Note that genotype == phenotype (in this haploid case)
  m_x_gen[0] = (Uniform() < 0.5 ? m.m_x_gen[0] : f.m_x_gen[0]) + Normal(0.0, sv);
  m_p_gen[0] = (Uniform() < 0.5 ? m.m_p_gen[0] : f.m_p_gen[0]) + Normal(0.0, sv);
  m_q_gen[0] = (Uniform() < 0.5 ? m.m_q_gen[0] : f.m_q_gen[0]) + Normal(0.0, sv);
  x=m_x_gen[0];
  p=m_p_gen[0];
  q=m_q_gen[0];
}

sado::indiv sado::create_offspring(const indiv& m, const indiv& f)
{
  indiv kid;
  kid.birth(m, f);
  return kid;
}

void sado::indiv::init(const double this_x0, const double this_p0, const double this_q0)
{
  //This is a bug (see https://github.com/richelbilderbeek/pbdmms/issues/163 ):
  //at initialization, the phenotype and genotype is unrelated
  m_x_gen[0]=this_x0+Normal(0.0,sv);
  m_p_gen[0]=this_p0+Normal(0.0,sv);
  m_q_gen[0]=this_q0+Normal(0.0,sv);
  x=this_x0+Normal(0.0,sv);
  p=this_p0+Normal(0.0,sv);
  q=this_q0+Normal(0.0,sv);
}

void sado::indiv::print() const noexcept
{
  std::cout << x << " " << p << " " << q << '\n';
  std::cout<<m_x_gen[0]<<" ";
  std::cout<<'\n';
  std::cout<<m_p_gen[0]<<" ";
  std::cout<<'\n';
  std::cout<<m_q_gen[0]<<" ";
  std::cout<<'\n';
}

bool sado::operator==(const indiv& lhs, const indiv& rhs) noexcept
{
  return lhs._x() == rhs._x()
    && lhs._p() == rhs._p()
    && lhs._q() == rhs._q()
  ;
}
