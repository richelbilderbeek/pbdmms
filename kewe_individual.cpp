#include "kewe_individual.h"

#include <cassert>
#include <stdexcept>
#include <iostream>
#include <random>
#include "kewe_helper.h"
#include "kewe_parameters.h"

kewe::individual::individual(
  const double eco_trait,
  const double fem_pref,
  const double male_trait,
  const std::vector<double>& eco_trait_loci,
  const std::vector<double>& fem_pref_loci,
  const std::vector<double>& male_trait_loci
) : m_eco_trait{eco_trait},
    m_eco_trait_loci{eco_trait_loci},
    m_fem_pref{fem_pref},
    m_fem_pref_loci{fem_pref_loci},
    m_male_trait{male_trait},
    m_male_trait_loci{male_trait_loci}
{

}

void kewe::birth_haploid_trait(
    const int i, //locus index
    std::vector<double>& trait,
    double& avg_trait,
    const std::vector<double>& m_trait,
    const std::vector<double>& f_trait,
    const simulation_parameters& p,
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

  std::normal_distribution<double> n_dis(0.0, p.get_mut_distr_width());
  // Mutate locus
  trait[i]+=n_dis(gen);
  avg_trait+=trait[i];

}

void kewe::birth_diploid_trait(
    const double i,
    std::vector<double>& trait,
    double& avg_trait,
    const std::vector<double>& m_trait,
    const std::vector<double>& f_trait,
    const simulation_parameters& p,
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

  std::normal_distribution<double> n_dis(0.0,p.get_mut_distr_width());
  // Mutate loci
  trait[i]+=n_dis(gen);
  trait[i+1]+=n_dis(gen);
  avg_trait+=trait[i]+trait[i+1];

}

void kewe::individual::birth_haploid(
    const individual& m,
    const individual& f,
    const simulation_parameters& p,
    std::mt19937& gen
    )
{
  const int sz = get_max(
    m_eco_trait_loci.size(),
    m_fem_pref_loci.size(),
    m_male_trait_loci.size()
  );

  for(int i=0;i!=sz;++i)
  {
    if (i < static_cast<int>(m_eco_trait_loci.size()))
    {
      birth_haploid_trait(
        i,
        m_eco_trait_loci,
        m_eco_trait,
        m.m_eco_trait_loci,
        f.m_eco_trait_loci,
        p,
        gen
      );
    }
    if (i < static_cast<int>(m_fem_pref_loci.size()))
    {
      birth_haploid_trait(
        i,
        m_fem_pref_loci,
        m_fem_pref,
        m.m_fem_pref_loci,
        f.m_fem_pref_loci,
        p,
        gen
      );
    }
    if (i < static_cast<int>(m_male_trait_loci.size()))
    {
      birth_haploid_trait(
        i,
        m_male_trait_loci,
        m_male_trait,
        m.m_male_trait_loci,
        f.m_male_trait_loci,
        p,
        gen
      );
    }
  }
}

void kewe::individual::birth_diploid(
    const individual& m,
    const individual& f,
    const simulation_parameters& p,
    std::mt19937& gen
    )
{
  const int maxSize = get_max(
    m_eco_trait_loci.size(),
    m_fem_pref_loci.size(),
    m_male_trait_loci.size()
  );

  for(int i=0;i<maxSize;i+=2)
    {
      if (i <= static_cast<int>(m_eco_trait_loci.size()-2))
      {
        birth_diploid_trait(
          i,
          m_eco_trait_loci,
          m_eco_trait,
          m.m_eco_trait_loci,
          f.m_eco_trait_loci,
          p,
          gen
        );
      }
      if (i <= static_cast<int>(m_fem_pref_loci.size()-2))
      {
        birth_diploid_trait(
          i,
          m_fem_pref_loci,
          m_fem_pref,
          m.m_fem_pref_loci,
          f.m_fem_pref_loci,
          p,
          gen
        );
      }
      if (i <= static_cast<int>(m_male_trait_loci.size()-2))
      {
        birth_diploid_trait(
          i, m_male_trait_loci,
          m_male_trait,
          m.m_male_trait_loci,
          f.m_male_trait_loci,
          p,
          gen
        );
      }
    }
}

kewe::individual kewe::create_offspring(
  const individual& father,
  const individual& mother,
  const simulation_parameters& p,
  std::mt19937& rng
)
{
  individual kid;
  kid.birth(mother, father, p, rng);
  return kid;
}


kewe::individual::individual(const simulation_parameters& p, std::mt19937& gen)
  : m_eco_trait{0.0},
    m_eco_trait_loci{std::vector<double>(1,0.0)},
    m_fem_pref{0.0},
    m_fem_pref_loci{std::vector<double>(1,0.0)},
    m_male_trait{0.0},
    m_male_trait_loci{std::vector<double>(1,0.0)}
{
  const double sv = p.get_mut_distr_width();
  const double x0{p.get_initial_eco_trait()};
  const double p0{p.get_initial_fem_pref()};
  const double q0{p.get_initial_male_trait()};

  const int Nx = m_eco_trait_loci.size();
  const int Np = m_fem_pref_loci.size();
  const int Nq = m_male_trait_loci.size();

  std::normal_distribution<double> n_dis(0.0,sv);
  // Initialize all loci to the 0value of the loci + a random mutation
  for(int i=0;i<Nx;++i)
  {
    assert(i >= 0);
    assert(i < static_cast<int>(m_eco_trait_loci.size()));
    m_eco_trait_loci[i]=x0+n_dis(gen);
  }
  for(int i=0;i<Np;++i)
  {
    assert(i >= 0);
    assert(i < static_cast<int>(m_fem_pref_loci.size()));
    m_fem_pref_loci[i]=p0+n_dis(gen);
  }
  for(int i=0;i<Nq;++i)
  {
    assert(i >= 0);
    assert(i < static_cast<int>(m_male_trait_loci.size()));
    m_male_trait_loci[i]=q0+n_dis(gen);
  }
  m_eco_trait=x0+n_dis(gen);
  m_fem_pref=p0+n_dis(gen);
  m_male_trait=q0+n_dis(gen);
}

// Make a new baby from male m and female f
void kewe::individual::birth(
    const individual& m,
    const individual& f,
    const simulation_parameters& p,
    std::mt19937& gen)
{
    m_eco_trait=0.0;
    m_fem_pref=0.0;
    m_male_trait=0.0;

    birth_haploid(m, f, p, gen);
    // Make average x, p and q
    m_eco_trait /= static_cast<int>(m_eco_trait_loci.size());
    m_fem_pref /= static_cast<int>(m_fem_pref_loci.size());
    m_male_trait /= static_cast<int>(m_male_trait_loci.size());
    return;
}

bool kewe::operator==(const individual& lhs, const individual& rhs) noexcept
{
  return
       lhs.get_eco_trait() == rhs.get_eco_trait()
    && lhs.get_fem_pref() == rhs.get_fem_pref()
    && lhs.get_male_trait() == rhs.get_male_trait()
    && lhs.get_eco_trait_loci() == rhs.get_eco_trait_loci()
    && lhs.get_fem_pref_loci() == rhs.get_fem_pref_loci()
    && lhs.get_male_trait_loci() == rhs.get_male_trait_loci()
  ;
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
