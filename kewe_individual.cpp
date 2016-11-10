#include "kewe_individual.h"

#include <cassert>
#include <stdexcept>
#include <iostream>
#include "kewe_random.h"
#include "kewe_parameters.h"

void indiv::birth_haploid_trait(
    const double i,
    std::vector<double>& trait,
    double& avg_trait,
    const std::vector<double>& m_trait,
    const std::vector<double>& f_trait,
    const kewe_parameters& parameters)
{

  assert(!trait.empty());
  assert(!m_trait.empty());
  assert(!f_trait.empty());

  // Pick mother's locus or father's locus
  if(Uniform()<0.5)
      trait[i]=m_trait[i];
  else
      trait[i]=f_trait[i];

  // Mutate locus
  trait[i]+=Normal(0.0,parameters.sv);
  avg_trait+=trait[i];

}

void indiv::birth_diploid_trait(
    const double i,
    std::vector<double>& trait,
    double& avg_trait,
    const std::vector<double>& m_trait,
    const std::vector<double>& f_trait,
    const kewe_parameters& parameters)
{
  // Pick one of each 2 mother's loci
  if(Uniform()<0.5)
      trait[i]=m_trait[i];
  else
      trait[i]=m_trait[i+1];
  // Pick one of each 2 father's loci
  if(Uniform()<0.5)
      trait[i+1]=f_trait[i];
  else
      trait[i+1]=f_trait[i+1];

  // Mutate loci
  trait[i]+=Normal(0.0,parameters.sv);
  trait[i+1]+=Normal(0.0,parameters.sv);
  avg_trait+=trait[i]+trait[i+1];

}

void indiv::birth_haploid(const indiv& m, const indiv& f, const kewe_parameters& parameters)
{
  int maxSize = std::max(static_cast<int>(X.size()), static_cast<int>(P.size()));
  maxSize = std::max(maxSize, static_cast<int>(Q.size()));

  for(int i=0;i<maxSize;i++)
    {
      if (i < static_cast<int>(X.size())) {birth_haploid_trait(i, X, x, m.X, f.X, parameters);}
      if (i < static_cast<int>(P.size())) {birth_haploid_trait(i, P, p, m.P, f.P, parameters);}
      if (i < static_cast<int>(Q.size())) {birth_haploid_trait(i, Q, q, m.Q, f.Q, parameters);}
    }


}

void indiv::birth_diploid(const indiv& m, const indiv& f, const kewe_parameters& parameters)
{
  int maxSize = std::max(static_cast<int>(X.size()), static_cast<int>(P.size()));
  maxSize = std::max(maxSize, static_cast<int>(Q.size()));

  for(int i=0;i<maxSize;i++)
    {
      if (i < static_cast<int>(X.size()-2)) {birth_diploid_trait(i, X, x, m.X, f.X, parameters);}
      if (i < static_cast<int>(P.size()-2)) {birth_diploid_trait(i, P, p, m.P, f.P, parameters);}
      if (i < static_cast<int>(Q.size()-2)) {birth_diploid_trait(i, Q, q, m.Q, f.Q, parameters);}
    }
}

indiv::indiv(const kewe_parameters& parameters)
{
    const int Nx = parameters.Nx;
    const int Np = parameters.Np;
    const int Nq = parameters.Nq;
    // Make vector of loci the size of the number of loci
    int i;
    X.resize(Nx);
    P.resize(Np);
    Q.resize(Nq);

    // Initialize them all as 0.0
    for(i=0;i<Nx;i++) X[i]=0.0;
    for(i=0;i<Np;i++) P[i]=0.0;
    for(i=0;i<Nq;i++) Q[i]=0.0;
    x=0.0; p=0.0; q=0.0;
    a=0.0;
    return;
}

indiv::indiv(const indiv &y)
{
    const int Nx = y.X.size();
    const int Np = y.P.size();
    const int Nq = y.Q.size();
    // Make vector of loci the size of the number of loci
    int i;
    X.resize(Nx);
    P.resize(Np);
    Q.resize(Nq);

    // set them all to the value of individual y.
    for(i=0;i<Nx;i++) X[i]=y.X[i];
    for(i=0;i<Np;i++) P[i]=y.P[i];
    for(i=0;i<Nq;i++) Q[i]=y.Q[i];
    x=y.x; p=y.p; q=y.q;
    a=y.a;
    return;
}

void indiv::init(const kewe_parameters& parameters)
{
    const double sv = parameters.sv;
    const double x0 = parameters.x0;
    const double p0 = parameters.p0;
    const double q0 = parameters.q0;

    const int Nx = X.size();
    const int Np = P.size();
    const int Nq = Q.size();

    int i;
    // Initialize all loci to the 0value of the loci + a random mutation
    for(i=0;i<Nx;i++) X[i]=x0+Normal(0.0,sv);
    for(i=0;i<Np;i++) P[i]=p0+Normal(0.0,sv);
    for(i=0;i<Nq;i++) Q[i]=q0+Normal(0.0,sv);
    x=x0+Normal(0.0,sv); p=p0+Normal(0.0,sv); q=q0+Normal(0.0,sv);
    return;
}

// Make a new baby from male m and female f
void indiv::birth(const indiv& m, const indiv& f, const kewe_parameters& parameters)
{
    x=0.0;
    p=0.0;
    q=0.0;

    if(parameters.haploid){birth_haploid(m, f, parameters);}

    if(parameters.diploid)
    {
      if(static_cast<int>(X.size()) < 2)
        throw std::invalid_argument("Cannot do diploid with 1 x locus");
      if(static_cast<int>(P.size()) < 2)
        throw std::invalid_argument("Cannot do diploid with 1 p locus");
      if(static_cast<int>(Q.size()) < 2)
        throw std::invalid_argument("Cannot do diploid with 1 q locus");

      birth_diploid(m, f, parameters);
   }
    // Make average x, p and q
    x /= static_cast<int>(X.size());
    p /= static_cast<int>(P.size());
    q /= static_cast<int>(Q.size());
    return;
}

void indiv::print(void)
{
    int i;
    std::cout<<x<<" "<<p<<" "<<q<<std::endl;
    for(i=0;i<static_cast<int>(X.size());i++) std::cout<<X[i]<<" ";
    std::cout<<std::endl;
    for(i=0;i<static_cast<int>(P.size());i++) std::cout<<P[i]<<" ";
    std::cout<<std::endl;
    for(i=0;i<static_cast<int>(Q.size());i++) std::cout<<Q[i]<<" ";
    std::cout << std::endl;
}

bool operator==(const indiv& lhs, const indiv& rhs) noexcept
{
    //STUB
    return lhs.X == rhs.X;

}
bool operator!=(const indiv& lhs, const indiv& rhs) noexcept
{
    return !(lhs == rhs);
}

