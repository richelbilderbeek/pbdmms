#include "lyke_parameters.h"

#include <cassert>
#include <string>
#include <iostream>
#include "lyke_random.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

lyke_parameters g_parameters;

lyke_parameters::lyke_parameters(
  const int    simulationruns,
  const int    L,
  const int    nGeneEco,
  const double mu,
  const double sigmaMut,
  const int    popSize,
  const double sigmac,
  const double sigmaK,
  const double alpha,
  const double beta,
  const int seed
)
  : m_simulationruns{simulationruns},
    m_L{L},
    m_nGeneEco{nGeneEco},
    m_mu{mu},
    m_sigmaMut{sigmaMut},
    m_popSize{popSize},
    m_sigmac{sigmac},
    m_sigmaK{sigmaK},
    m_alpha{alpha},
    m_beta{beta},
    m_seed{seed}
{
  assert(m_simulationruns >= 0);
  assert(m_L >= 0);
  assert(m_nGeneEco >= 0);
  assert(m_mu >= 0.0);
  assert(m_sigmaMut >= 0.0);
  assert(m_popSize >= 0);
  assert(m_sigmac >= 0.0);
  assert(m_sigmaK >= 0.0);
  assert(m_alpha >= 0.0);
  assert(m_beta >= 0.0);
  assert(m_seed>= 0.0);
}

int lyke_parameters::get_simulationruns() const noexcept
{
  return this->m_simulationruns;
}

int lyke_parameters::get_L() const noexcept
{
  return this->m_L;
}

int lyke_parameters::get_nGeneEco() const noexcept
{
  return this->m_nGeneEco;
}

double lyke_parameters::get_mu() const noexcept
{
  return this->m_mu;
}

double lyke_parameters::get_sigmaMut() const noexcept
{
  return this->m_sigmaMut;
}

double lyke_parameters::get_sigmac() const noexcept
{
  return this->m_sigmac;
}

double lyke_parameters::get_sigmaK() const noexcept
{
  return this->m_sigmaK;
}

double lyke_parameters::get_alpha() const noexcept
{
  return this->m_alpha;
}

double lyke_parameters::get_beta() const noexcept
{
  return this->m_beta;
}

int lyke_parameters::get_popSize() const noexcept
{
  return this->m_popSize;
}

int create_random_seed() noexcept
{
  std::srand(time(NULL));
  return std::rand();
}

lyke_parameters read_parameters_from_file(const std::string& filename)
{
  std::clog << "STUB! read_parameters_from_file: " << filename << '\n';
  return lyke_parameters();
}

std::ostream& operator<<(std::ostream& os, const lyke_parameters p) noexcept
{
  os
    << "Version:" << p.get_version() << '\n'
    << "simulationruns:" << p.get_simulationruns() << '\n'
    << "L:" << p.get_L() << '\n'
    << "nGeneEco:" << p.get_nGeneEco() << '\n'
    << "mu:" << p.get_mu() << '\n'
    << "sigmaMut:" << p.get_sigmaMut() << '\n'
    << "popSize:" << p.get_popSize() << '\n'
    << "sigmac:" << p.get_sigmac() << '\n'
    << "sigmaK:" << p.get_sigmaK() << '\n'
    << "alpha: " << p.get_alpha() << '\n'
    << "beta:" << p.get_alpha() << '\n'
  ;
  return os;
}

