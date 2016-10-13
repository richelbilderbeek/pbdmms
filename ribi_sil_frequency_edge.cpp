#include "ribi_sil_frequency_edge.h"
#include <sstream>
#include <stdexcept>

ribi::sil_frequency_edge::sil_frequency_edge(
  const int n_timesteps,
  const int n_edges
) : m_n_edges{n_edges},
    m_n_timesteps{n_timesteps}
{
  if (m_n_edges < 1)
  {
    throw std::invalid_argument("n_edges must be at least one");
  }
  if (m_n_timesteps < 0)
  {
    throw std::invalid_argument("n_timesteps must be at least zero");
  }
}
