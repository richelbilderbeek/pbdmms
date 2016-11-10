#ifndef PBD_L_TABLE_ROW_H
#define PBD_L_TABLE_ROW_H

namespace pbd {

class l_table_row
{
public:
  /// @param m_incip_lvl_lbl incipient-level label of a species
  /// @param m_incip_lvl_lbl_par the incipient-level label
  ///   of the parent of the species
  /// @param m_t_incipient the time at which a species is
  ///   born as incipient species
  /// @param m_t_good the time of speciation-completion of the species.
  ///   If this value equals -1, then the species is still incipient
  /// @param m_t_extinction the time of extinction of the species.
  ///   If this value equals -1, then the species is still extant
  /// @param m_species_level_label the species-level label of the species
  /// @param
  /// @param
  // - the third column is
  explicit l_table_row(
    const int incip_lvl_lbl,
    const int incip_lvl_lbl_par,
    const double t_incipient,
    const double t_good,
    const double t_extinction,
    const int species_level_label
  );

  /// incipient-level label of a species
  const int m_incip_lvl_lbl;

  /// the incipient-level label of the parent of the species
  const int m_incip_lvl_lbl_par;

  /// the time at which a species is born as incipient species
  const double m_t_incipient;

  /// the time of speciation-completion of the species.
  /// If -1, then the species is still incipient.
  const double m_t_good;

  /// the time of extinction of the species
  /// If -1, then the species is still extant.
  const double m_t_extinction;

  /// the species-level label of the species
  const int m_species_level_label;
};

} //~namespace pbd

#endif // PBD_L_TABLE_ROW_H
