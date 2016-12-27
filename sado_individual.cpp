#include "sado_individual.h"

#include <iostream>

sado::indiv::indiv()
{
  X.resize(Nx, 0.0);
  P.resize(Np, 0.0);
  Q.resize(Nq, 0.0);
  x=0.0;
  p=0.0;
  q=0.0;
  a=0.0;
}

void sado::indiv::birth(const indiv& m, const indiv& f)
{
  x=0.0;
  p=0.0;
  q=0.0;
  if(haploid)
  {
    for(int i=0;i<Nx;i++)
    {
      if(Uniform()<0.5) X[i]=m.X[i];
      else X[i]=f.X[i];
      X[i]+=Normal(0.0,sv);
      x+=X[i];
    }
    for(int i=0;i<Np;i++)
    {
      if(Uniform()<0.5) P[i]=m.P[i];
      else P[i]=f.P[i];
      P[i]+=Normal(0.0,sv);
      p+=P[i];
    }
    for(int i=0;i<Nq;i++)
    {
      if(Uniform()<0.5) Q[i]=m.Q[i];
      else Q[i]=f.Q[i];
      Q[i]+=Normal(0.0,sv);
      q+=Q[i];
    }
  }
  if(diploid)
  {
    for(int i=0;i<=Nx-2;i+=2)
    {
      if(Uniform()<0.5) X[i]=m.X[i];
      else X[i]=m.X[i+1];
      if(Uniform()<0.5) X[i+1]=f.X[i];
      else X[i+1]=f.X[i+1];
      X[i]+=Normal(0.0,sv);
      X[i+1]+=Normal(0.0,sv);
      x+=X[i]+X[i+1];
    }
    for(int i=0;i<=Np-2;i+=2)
    {
      if(Uniform()<0.5) P[i]=m.P[i];
      else P[i]=m.P[i+1];
      if(Uniform()<0.5) P[i+1]=f.P[i];
      else P[i+1]=f.P[i+1];
      P[i]+=Normal(0.0,sv);
      P[i+1]+=Normal(0.0,sv);
      p+=P[i]+P[i+1];
    }
    for(int i=0;i<=Nq-2;i+=2)
    {
      if(Uniform()<0.5) Q[i]=m.Q[i];
      else Q[i]=m.Q[i+1];
      if(Uniform()<0.5) Q[i+1]=f.Q[i];
      else Q[i+1]=f.Q[i+1];
      Q[i]+=Normal(0.0,sv);
      Q[i+1]+=Normal(0.0,sv);
      q+=Q[i]+Q[i+1];
    }
  }
  x/=Nx;
  p/=Np;
  q/=Nq;
}

sado::indiv sado::create_offspring(const indiv& m, const indiv& f)
{
  indiv kid;
  kid.birth(m, f);
  return kid;
}

void sado::indiv::init(double this_x0, double this_p0, double this_q0)
{
  for(int i=0;i<Nx;i++) X[i]=this_x0+Normal(0.0,sv);
  for(int i=0;i<Np;i++) P[i]=this_p0+Normal(0.0,sv);
  for(int i=0;i<Nq;i++) Q[i]=this_q0+Normal(0.0,sv);
  x=this_x0+Normal(0.0,sv);
  p=this_p0+Normal(0.0,sv);
  q=this_q0+Normal(0.0,sv);
}

void sado::indiv::print() const noexcept
{
  std::cout << x << " " << p << " " << q << '\n';
  for(int i=0;i<Nx;i++) std::cout<<X[i]<<" ";
  std::cout<<'\n';
  for(int i=0;i<Np;i++) std::cout<<P[i]<<" ";
  std::cout<<'\n';
  for(int i=0;i<Nq;i++) std::cout<<Q[i]<<" ";
  std::cout<<'\n';
}

bool sado::operator==(const indiv& lhs, const indiv& rhs) noexcept
{
  return lhs._x() == rhs._x()
    && lhs._p() == rhs._p()
    && lhs._q() == rhs._q()
  ;
}
