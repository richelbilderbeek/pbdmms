#ifndef PBD_L_TABLE_ROW_H
#define PBD_L_TABLE_ROW_H

namespace pbd {

class l_table_row
{
public:
  explicit l_table_row(
    const int incipient_level_label,
    const int incipient_level_label_parents,
    const double t_incipient,
    const double t_good,
    const double t_extinction,
    const int species_level_label
  );

  // - the first column is the incipient-level label of a species
  const int m_incip_lvl_lbl;
  // - the second column is the incipient-level label of the parent of the species
  const int m_incip_lvl_lbl_par;
  // - the third column is the time at which a species is born as incipient species
  const double m_t_incipient;
  // - the fourth column is the time of speciation-completion of the species
  //   If the fourth element equals -1, then the species is still incipient.
  const double m_t_good;
  // - the fifth column is the time of extinction of the species
  //   If the fifth element equals -1, then the species is still extant.
  const double m_t_extinction;
  // - The sixth column is the species-level label of the species
  const int m_species_level_label;
};

} //~namespace pbd

#endif // PBD_L_TABLE_ROW_H
