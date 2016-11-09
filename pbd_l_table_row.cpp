#include "pbd_l_table_row.h"

#include <cassert>
#include <cmath>
#include <stdexcept>

pbd::l_table_row::l_table_row(
  const int incipient_level_label,
  const int incipient_level_label_parents,
  const double t_incipient,
  const double t_good,
  const double t_extinction,
  const int species_level_label
) : m_incipient_level_label{incipient_level_label},
    m_incipient_level_label_parents{incipient_level_label_parents},
    m_t_incipient{t_incipient},
    m_t_good{t_good},
    m_t_extinction{t_extinction},
    m_species_level_label{species_level_label}
{
  if (m_species_level_label <= 0)
  {
    throw std::invalid_argument("species level label must be one or more");
  }
  if (m_incipient_level_label_parents < 0)
  {
    throw std::invalid_argument("incipient level label must be zero or more");
  }
  if (m_t_incipient < 0.0 && std::abs(m_t_incipient - -1.0) < 0.01)
  {
    throw std::invalid_argument("incipient level label must be zero or more");
  }
  if (m_t_good < 0.0 && std::abs(m_t_good - -1.0) > 0.01)
  {
    throw std::invalid_argument("t_good must be positive or -1");
  }
  if (m_t_extinction < 0.0 && std::abs(m_t_extinction - -1.0) > 0.01)
  {
    throw std::invalid_argument("t_extinction must be positive or -1");
  }
  if (m_species_level_label <= 0)
  {
    throw std::invalid_argument("species label must be one or more");
  }

  assert(m_species_level_label > 0);
  assert(m_incipient_level_label_parents >= 0);
  assert(m_t_incipient >= 0.0 || std::abs(m_t_incipient - -1.0) < 0.001);
  assert(m_t_good >= 0.0 || std::abs(m_t_good - -1.0) < 0.001);
  assert(m_t_extinction >= 0.0 || std::abs(m_t_extinction - -1.0) < 0.001);
  assert(m_species_level_label > 0);
}
