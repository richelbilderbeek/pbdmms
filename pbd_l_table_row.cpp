#include "pbd_l_table_row.h"

#include <cassert>
#include <cmath>

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
  assert(m_species_level_label > 0);
  assert(m_incipient_level_label_parents > 0);
  assert(m_t_incipient >= 0.0 || std::abs(m_t_incipient - 1.0) < 0.001);
  assert(m_t_good >= 0.0 || std::abs(m_t_good - 1.0) < 0.001);
  assert(m_t_extinction >= 0.0 || std::abs(m_t_extinction - 1.0) < 0.001);
  assert(m_species_level_label > 0);
}
