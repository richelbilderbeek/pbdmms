#include "lyke_parameters.h"

#include <cassert>

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
  const double beta
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
    m_beta{beta}
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
}

int lyke_parameters::get_simulationruns() const noexcept
{
  return this->m_simulationruns;
}

int lyke_parameters::get_nGeneEco() const noexcept
{
  return this->m_nGeneEco;
}
