#include "kewe_individual.h"

#include <cassert>
#include <stdexcept>
#include <iostream>
#include <random>
#include "kewe_parameters.h"

void indiv::birth_haploid_trait(
    const double i,
    std::vector<double>& trait,
    double& avg_trait,
    const std::vector<double>& m_trait,
    const std::vector<double>& f_trait,
    const kewe_parameters& parameters,
    std::mt19937& gen
    )
{

  assert(!trait.empty());
  assert(!m_trait.empty());
  assert(!f_trait.empty());

  std::uniform_real_distribution<> dis(0, 1);

  // Pick mother's locus or father's locus
  if(dis(gen)<0.5)
      trait[i]=m_trait[i];
  else
      trait[i]=f_trait[i];

  std::normal_distribution<double> n_dis(0.0,parameters.sim_parameters.sv);
  // Mutate locus
  trait[i]+=n_dis(gen);
  avg_trait+=trait[i];

}

void indiv::birth_diploid_trait(
    const double i,
    std::vector<double>& trait,
    double& avg_trait,
    const std::vector<double>& m_trait,
    const std::vector<double>& f_trait,
    const kewe_parameters& parameters,
    std::mt19937& gen
    )
{
  // Pick one of each 2 mother's loci
  std::uniform_real_distribution<> dis(0,1);
  if(dis(gen)<0.5)
      trait[i]=m_trait[i];
  else
      trait[i]=m_trait[i+1];
  // Pick one of each 2 father's loci
  if(dis(gen)<0.5)
      trait[i+1]=f_trait[i];
  else
      trait[i+1]=f_trait[i+1];

  std::normal_distribution<double> n_dis(0.0,parameters.sim_parameters.sv);
  // Mutate loci
  trait[i]+=n_dis(gen);
  trait[i+1]+=n_dis(gen);
  avg_trait+=trait[i]+trait[i+1];

}

void indiv::birth_haploid(
    const indiv& m,
    const indiv& f,
    const kewe_parameters& parameters,
    std::mt19937& gen
    )
{
  int maxSize = std::max(static_cast<int>(X.size()), static_cast<int>(P.size()));
  maxSize = std::max(maxSize, static_cast<int>(Q.size()));

  for(int i=0;i<maxSize;i++)
    {
      if (i < static_cast<int>(X.size())) {birth_haploid_trait(i, X, x, m.X, f.X, parameters, gen);}
      if (i < static_cast<int>(P.size())) {birth_haploid_trait(i, P, p, m.P, f.P, parameters, gen);}
      if (i < static_cast<int>(Q.size())) {birth_haploid_trait(i, Q, q, m.Q, f.Q, parameters, gen);}
    }


}

void indiv::birth_diploid(
    const indiv& m,
    const indiv& f,
    const kewe_parameters& parameters,
    std::mt19937& gen
    )
{
  int maxSize = std::max(static_cast<int>(X.size()), static_cast<int>(P.size()));
  maxSize = std::max(maxSize, static_cast<int>(Q.size()));

  for(int i=0;i<maxSize;i+=2)
    {
      if (i <= static_cast<int>(X.size()-2))
        birth_diploid_trait(i, X, x, m.X, f.X, parameters, gen);
      if (i <= static_cast<int>(P.size()-2))
        birth_diploid_trait(i, P, p, m.P, f.P, parameters, gen);
      if (i <= static_cast<int>(Q.size()-2))
        birth_diploid_trait(i, Q, q, m.Q, f.Q, parameters, gen);
    }
}

indiv::indiv(const kewe_parameters& parameters)
  : X{std::vector<double>(parameters.sim_parameters.Nx,0.0)},
    P{std::vector<double>(parameters.sim_parameters.Np,0.0)},
    Q{std::vector<double>(parameters.sim_parameters.Nq,0.0)},
    x{0.0},
    p{0.0},
    q{0.0},
    a{0.0}

{}

void indiv::init(const kewe_parameters& parameters, std::mt19937& gen)
{
    const double sv = parameters.sim_parameters.sv;
    const double x0 = parameters.sim_parameters.x0;
    const double p0 = parameters.sim_parameters.p0;
    const double q0 = parameters.sim_parameters.q0;

    const int Nx = X.size();
    const int Np = P.size();
    const int Nq = Q.size();

    std::normal_distribution<double> n_dis(0.0,sv);
    // Initialize all loci to the 0value of the loci + a random mutation
    for(int i=0;i<Nx;i++) X[i]=x0+n_dis(gen);
    for(int i=0;i<Np;i++) P[i]=p0+n_dis(gen);
    for(int i=0;i<Nq;i++) Q[i]=q0+n_dis(gen);
    x=x0+n_dis(gen); p=p0+n_dis(gen); q=q0+n_dis(gen);
}

// Make a new baby from male m and female f
void indiv::birth(
    const indiv& m,
    const indiv& f,
    const kewe_parameters& parameters,
    std::mt19937& gen)
{
    x=0.0;
    p=0.0;
    q=0.0;

    if(parameters.sim_parameters.haploid){birth_haploid(m, f, parameters, gen);}

    if(parameters.sim_parameters.diploid)
    {
      if(static_cast<int>(X.size()) < 2)
        throw std::invalid_argument("Cannot do diploid with 1 x locus");
      if(static_cast<int>(P.size()) < 2)
        throw std::invalid_argument("Cannot do diploid with 1 p locus");
      if(static_cast<int>(Q.size()) < 2)
        throw std::invalid_argument("Cannot do diploid with 1 q locus");

      birth_diploid(m, f, parameters, gen);
   }
    // Make average x, p and q
    x /= static_cast<int>(X.size());
    p /= static_cast<int>(P.size());
    q /= static_cast<int>(Q.size());
    return;
}

bool operator==(const indiv& lhs, const indiv& rhs) noexcept
{
    return lhs.X == rhs.X && lhs.P == rhs.P && lhs.Q == rhs.Q;
}
bool operator!=(const indiv& lhs, const indiv& rhs) noexcept
{
    return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, const indiv& /* i */) noexcept
{
  //STUB
  os << "STUB";
  return os;
}
