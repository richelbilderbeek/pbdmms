#ifndef SADO_INDIVIDUAL_H
#define SADO_INDIVIDUAL_H

#include <list>
#include "sado_globals.h"
#include "sado_random.h"

namespace sado {

class indiv
{
private:
  double x,p,q,*X,*P,*Q,a;
public:
  ~indiv() { delete[] X; delete[] P; delete[] Q; return;}
  indiv()
  {
    X=new double[Nx];
    P=new double[Np];
    Q=new double[Nq];
    for(int i=0;i<Nx;i++) X[i]=0.0;
    for(int i=0;i<Np;i++) P[i]=0.0;
    for(int i=0;i<Nq;i++) Q[i]=0.0;
    x=0.0;
    p=0.0;
    q=0.0;
    a=0.0;
  }
  indiv(const indiv &y)
  {
    X=new double[Nx];
    P=new double[Np];
    Q=new double[Nq];
    for(int i=0;i<Nx;i++) X[i]=y.X[i];
    for(int i=0;i<Np;i++) P[i]=y.P[i];
    for(int i=0;i<Nq;i++) Q[i]=y.Q[i];
    x=y.x;
    p=y.p;
    q=y.q;
    a=y.a;
  }
  void init(double this_x0, double this_p0, double this_q0)
  {
    for(int i=0;i<Nx;i++) X[i]=this_x0+Normal(0.0,sv);
    for(int i=0;i<Np;i++) P[i]=this_p0+Normal(0.0,sv);
    for(int i=0;i<Nq;i++) Q[i]=this_q0+Normal(0.0,sv);
    x=this_x0+Normal(0.0,sv);
    p=this_p0+Normal(0.0,sv);
    q=this_q0+Normal(0.0,sv);
  }
  void birth(indiv m, indiv f)
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
  void print();
  double _x() const noexcept { return x;}
  double _p() const noexcept { return p;}
  double _q() const noexcept { return q;}
  double _a() const noexcept { return a;}
  void a_(double A) { a=A; return;}

};

bool operator==(const indiv& lhs, const indiv& rhs) noexcept;

} //~namespace sado

#endif // SADO_INDIVIDUAL_H
