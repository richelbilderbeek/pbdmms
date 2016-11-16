#include "pbd_parameters.h"

#include <cassert>

pbd::parameters::parameters(
  const double birth_good,
  const double birth_incipient,
  const double completion,
  const double death_good,
  const double death_incipient,
  const double time,
  const int seed
)
  : m_birth_good{birth_good},
    m_birth_incipient{birth_incipient},
    m_completion{completion},
    m_death_good{death_good},
    m_death_incipient{death_incipient},
    m_time{time},
    m_seed{seed}
{
  assert(m_birth_good >= 0.0);
  assert(m_birth_incipient >= 0.0);
  assert(m_completion >= 0.0);
  assert(m_death_good >= 0.0);
  assert(m_death_incipient >= 0.0);
  assert(m_time >= 0.0);
}
