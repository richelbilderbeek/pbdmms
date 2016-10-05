#include "distancer_sil_frequency_edge.h"
#include <sstream>
#include <stdexcept>

sil_frequency_edge::sil_frequency_edge()
  : m_n_timesteps{0}
{

}


sil_frequency_edge::sil_frequency_edge(
  const int n_timesteps
) : m_n_timesteps{n_timesteps}
{
  if (m_n_timesteps < 0)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "n_timesteps must be zero or and positive, instead of "
      << m_n_timesteps
    ;
    throw std::invalid_argument(msg.str());
  }
}
