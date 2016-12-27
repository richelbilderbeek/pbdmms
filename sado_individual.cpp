#include "sado_individual.h"
#include "sado_random.h"

#include <iostream>

sado::indiv::indiv()
{
  m_x=0.0;
  m_p=0.0;
  m_q=0.0;
  m_a=0.0;
}

void sado::indiv::birth(const indiv& m, const indiv& f)
{
  //Note that genotype == phenotype (in this haploid case)
  m_x_gen = (Uniform() < 0.5 ? m.m_x_gen : f.m_x_gen) + Normal(0.0, sv);
  m_p_gen = (Uniform() < 0.5 ? m.m_p_gen : f.m_p_gen) + Normal(0.0, sv);
  m_q_gen = (Uniform() < 0.5 ? m.m_q_gen : f.m_q_gen) + Normal(0.0, sv);
  m_x=m_x_gen;
  m_p=m_p_gen;
  m_q=m_q_gen;
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
  m_x_gen=this_x0+Normal(0.0,sv);
  m_p_gen=this_p0+Normal(0.0,sv);
  m_q_gen=this_q0+Normal(0.0,sv);
  m_x=this_x0+Normal(0.0,sv);
  m_p=this_p0+Normal(0.0,sv);
  m_q=this_q0+Normal(0.0,sv);
}


std::ostream& sado::operator<<(std::ostream& os, const indiv i) noexcept
{
  os << i.m_x << " " << i.m_p << " " << i.m_q << '\n';
  os << i.m_x_gen <<" ";
  os << '\n';
  os << i.m_p_gen <<" ";
  os << '\n';
  os << i.m_q_gen <<" ";
  os << '\n';
  return os;
}

bool sado::operator==(const indiv& lhs, const indiv& rhs) noexcept
{
  return lhs.get_x() == rhs.get_x()
    && lhs.get_p() == rhs.get_p()
    && lhs.get_q() == rhs.get_q()
  ;
}
