#include "pbd_l_table_row.h"

#include <cassert>
#include <cmath>
#include <stdexcept>

pbd::l_table_row::l_table_row(
  const int incip_lvl_lbl,
  const int incip_lvl_lbl_par,
  const double t_incipient,
  const double t_good,
  const double t_extinction,
  const int species_level_label
) : m_incip_lvl_lbl{incip_lvl_lbl},
    m_incip_lvl_lbl_par{incip_lvl_lbl_par},
    m_t_incipient{t_incipient},
    m_t_good{t_good},
    m_t_extinction{t_extinction},
    m_species_level_label{species_level_label}
{
  if (incip_lvl_lbl <= 0)
  {
    throw std::invalid_argument("incipient level label must be one or more");
  }
  //incipient_level_label_parents can be positive, zero or negative
  if (m_t_incipient < 0.0 && std::abs(m_t_incipient - -1.0) > 0.01)
  {
    throw std::invalid_argument("t_incipient must be postive or -1");
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
}
