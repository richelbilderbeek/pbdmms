#include "kewe_individual.h"

#include <cassert>
#include <stdexcept>
#include <iostream>
#include <random>
#include "kewe_parameters.h"

void kewe::individual::birth_haploid_trait(
    const int i, //locus index
    std::vector<double>& trait,
    double& avg_trait,
    const std::vector<double>& m_trait,
    const std::vector<double>& f_trait,
    const parameters& parameters,
    std::mt19937& gen
    )
{
  assert(!trait.empty());
  assert(!m_trait.empty());
  assert(m_trait.size() == f_trait.size());

  std::uniform_real_distribution<> dis(0.0, 1.0);
  assert(i >= 0);
  assert(i < static_cast<int>(trait.size()));

  // Pick mother's locus or father's locus
  if(dis(gen)<0.5)
      trait[i]=m_trait[i];
  else
      trait[i]=f_trait[i];

  std::normal_distribution<double> n_dis(0.0, parameters.m_sim_parameters.sv);
  // Mutate locus
  trait[i]+=n_dis(gen);
  avg_trait+=trait[i];

}

void kewe::individual::birth_diploid_trait(
    const double i,
    std::vector<double>& trait,
    double& avg_trait,
    const std::vector<double>& m_trait,
    const std::vector<double>& f_trait,
    const parameters& parameters,
    std::mt19937& gen
    )
{

  assert(i >= 0);
  assert(i < static_cast<int>(trait.size()));
  assert(i + 1 < static_cast<int>(trait.size()));

  assert(i < static_cast<int>(m_trait.size()));
  assert(i + 1 < static_cast<int>(m_trait.size()));

  assert(i < static_cast<int>(f_trait.size()));
  assert(i + 1 < static_cast<int>(f_trait.size()));
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

  std::normal_distribution<double> n_dis(0.0,parameters.m_sim_parameters.sv);
  // Mutate loci
  trait[i]+=n_dis(gen);
  trait[i+1]+=n_dis(gen);
  avg_trait+=trait[i]+trait[i+1];

}

void kewe::individual::birth_haploid(
    const individual& m,
    const individual& f,
    const parameters& parameters,
    std::mt19937& gen
    )
{
  int maxSize = std::max(static_cast<int>(m_X.size()), static_cast<int>(m_P.size()));
  maxSize = std::max(maxSize, static_cast<int>(m_Q.size()));

  for(int i=0;i<maxSize;i++)
    {
      if (i < static_cast<int>(m_X.size()))
      {
        birth_haploid_trait(i, m_X, m_x, m.m_X, f.m_X, parameters, gen);
      }
      if (i < static_cast<int>(m_P.size()))
      {
        birth_haploid_trait(i, m_P, m_p, m.m_P, f.m_P, parameters, gen);
      }
      if (i < static_cast<int>(m_Q.size()))
      {
        birth_haploid_trait(i, m_Q, m_q, m.m_Q, f.m_Q, parameters, gen);
      }
    }


}

void kewe::individual::birth_diploid(
    const individual& m,
    const individual& f,
    const parameters& parameters,
    std::mt19937& gen
    )
{
  int maxSize = std::max(static_cast<int>(m_X.size()), static_cast<int>(m_P.size()));
  maxSize = std::max(maxSize, static_cast<int>(m_Q.size()));

  for(int i=0;i<maxSize;i+=2)
    {
      if (i <= static_cast<int>(m_X.size()-2))
        birth_diploid_trait(i, m_X, m_x, m.m_X, f.m_X, parameters, gen);
      if (i <= static_cast<int>(m_P.size()-2))
        birth_diploid_trait(i, m_P, m_p, m.m_P, f.m_P, parameters, gen);
      if (i <= static_cast<int>(m_Q.size()-2))
        birth_diploid_trait(i, m_Q, m_q, m.m_Q, f.m_Q, parameters, gen);
    }
}

kewe::individual::individual(const parameters& parameters)
  : m_X{std::vector<double>(parameters.m_sim_parameters.Nx,0.0)},
    m_P{std::vector<double>(parameters.m_sim_parameters.Np,0.0)},
    m_Q{std::vector<double>(parameters.m_sim_parameters.Nq,0.0)},
    m_x{0.0},
    m_p{0.0},
    m_q{0.0}
{

}

void kewe::individual::init(const parameters& parameters, std::mt19937& gen)
{
    const double sv = parameters.m_sim_parameters.sv; //width distribution mutation sizes
    const double x0 = parameters.m_sim_parameters.x0;
    const double p0 = parameters.m_sim_parameters.p0;
    const double q0 = parameters.m_sim_parameters.q0;

    const int Nx = m_X.size();
    const int Np = m_P.size();
    const int Nq = m_Q.size();


    std::normal_distribution<double> n_dis(0.0,sv);
    // Initialize all loci to the 0value of the loci + a random mutation
    for(int i=0;i<Nx;i++)
      {
        assert(i >= 0);
        assert(i < static_cast<int>(m_X.size()));
        m_X[i]=x0+n_dis(gen);
      }
    for(int i=0;i<Np;i++)
      {
        assert(i >= 0);
        assert(i < static_cast<int>(m_P.size()));
        m_P[i]=p0+n_dis(gen);
      }
    for(int i=0;i<Nq;i++)
      {
        assert(i >= 0);
        assert(i < static_cast<int>(m_Q.size()));
        m_Q[i]=q0+n_dis(gen);
      }
    m_x=x0+n_dis(gen);
    m_p=p0+n_dis(gen);
    m_q=q0+n_dis(gen);
}

// Make a new baby from male m and female f
void kewe::individual::birth(
    const individual& m,
    const individual& f,
    const parameters& parameters,
    std::mt19937& gen)
{
    m_x=0.0;
    m_p=0.0;
    m_q=0.0;

    if(parameters.m_sim_parameters.haploid)
    {
      birth_haploid(m, f, parameters, gen);
    }

    if(parameters.m_sim_parameters.diploid)
    {
      if(static_cast<int>(m_X.size()) < 2)
        throw std::invalid_argument("Cannot do diploid with 1 x locus");
      if(static_cast<int>(m_P.size()) < 2)
        throw std::invalid_argument("Cannot do diploid with 1 p locus");
      if(static_cast<int>(m_Q.size()) < 2)
        throw std::invalid_argument("Cannot do diploid with 1 q locus");

      birth_diploid(m, f, parameters, gen);
   }
    // Make average x, p and q
    m_x /= static_cast<int>(m_X.size());
    m_p /= static_cast<int>(m_P.size());
    m_q /= static_cast<int>(m_Q.size());
    return;
}

bool kewe::operator==(const individual& lhs, const individual& rhs) noexcept
{
    return lhs.m_X == rhs.m_X && lhs.m_P == rhs.m_P && lhs.m_Q == rhs.m_Q;
}
bool kewe::operator!=(const individual& lhs, const individual& rhs) noexcept
{
    return !(lhs == rhs);
}

std::ostream& kewe::operator<<(std::ostream& os, const individual& i) noexcept
{

  os << "i_x: " << i.get_eco_trait()
     << " i_p: " << i.get_fem_pref()
     << " i_q: " << i.get_male_trait();
  return os;
}
