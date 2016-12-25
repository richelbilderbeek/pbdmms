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
    int i;
    X=new double[Nx];
    P=new double[Np];
    Q=new double[Nq];
    for(i=0;i<Nx;i++) X[i]=0.0;
    for(i=0;i<Np;i++) P[i]=0.0;
    for(i=0;i<Nq;i++) Q[i]=0.0;
    x=0.0; p=0.0; q=0.0;
    a=0.0;
    return;
  }
  indiv(const indiv &y)
  {
    int i;
    X=new double[Nx];
    P=new double[Np];
    Q=new double[Nq];
    for(i=0;i<Nx;i++) X[i]=y.X[i];
    for(i=0;i<Np;i++) P[i]=y.P[i];
    for(i=0;i<Nq;i++) Q[i]=y.Q[i];
    x=y.x; p=y.p; q=y.q;
    a=y.a;
    return;
  }
  void init(double x0, double p0, double q0)
  {
    int i;
    for(i=0;i<Nx;i++) X[i]=x0+Normal(0.0,sv);
    for(i=0;i<Np;i++) P[i]=p0+Normal(0.0,sv);
    for(i=0;i<Nq;i++) Q[i]=q0+Normal(0.0,sv);
    x=x0+Normal(0.0,sv); p=p0+Normal(0.0,sv); q=q0+Normal(0.0,sv);
    return;
  }
  void birth(indiv m, indiv f)
  {
    int i;
    x=0.0;
    p=0.0;
    q=0.0;
    if(haploid)
      {
        for(i=0;i<Nx;i++)
          {
            if(Uniform()<0.5) X[i]=m.X[i];
            else X[i]=f.X[i];
            X[i]+=Normal(0.0,sv);
            x+=X[i];
          }
        for(i=0;i<Np;i++)
          {
            if(Uniform()<0.5) P[i]=m.P[i];
            else P[i]=f.P[i];
            P[i]+=Normal(0.0,sv);
            p+=P[i];
          }
        for(i=0;i<Nq;i++)
          {
            if(Uniform()<0.5) Q[i]=m.Q[i];
            else Q[i]=f.Q[i];
            Q[i]+=Normal(0.0,sv);
            q+=Q[i];
          }
      }
    if(diploid)
      {
        for(i=0;i<=Nx-2;i+=2)
          {
            if(Uniform()<0.5) X[i]=m.X[i];
            else X[i]=m.X[i+1];
            if(Uniform()<0.5) X[i+1]=f.X[i];
            else X[i+1]=f.X[i+1];
            X[i]+=Normal(0.0,sv);
            X[i+1]+=Normal(0.0,sv);
            x+=X[i]+X[i+1];
          }
        for(i=0;i<=Np-2;i+=2)
          {
            if(Uniform()<0.5) P[i]=m.P[i];
            else P[i]=m.P[i+1];
            if(Uniform()<0.5) P[i+1]=f.P[i];
            else P[i+1]=f.P[i+1];
            P[i]+=Normal(0.0,sv);
            P[i+1]+=Normal(0.0,sv);
            p+=P[i]+P[i+1];
          }
        for(i=0;i<=Nq-2;i+=2)
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
    return;
  }
  void print();
  double _x() { return x;}
  double _p() { return p;}
  double _q() { return q;}
  double _a() { return a;}
  void a_(double A) { a=A; return;}

};

} //~namespace sado

#endif // SADO_INDIVIDUAL_H
