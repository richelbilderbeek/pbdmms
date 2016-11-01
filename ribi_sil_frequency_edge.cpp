#include "ribi_sil_frequency_edge.h"

#include <cassert>
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

void ribi::sil_frequency_edge::clear() noexcept
{
  m_n_edges = 0;
  m_n_timesteps = 0;
}

void ribi::move_sil_frequencies(sil_frequency_edge& from, sil_frequency_edge& to)
{
  assert(from.get_n_timesteps() == to.get_n_timesteps());
  const auto n_timesteps = from.get_n_timesteps();
  const auto n_edges = from.get_n_edges() + to.get_n_edges();
  to = sil_frequency_edge(n_timesteps, n_edges);
  from.clear();
}
