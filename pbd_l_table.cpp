#include "pbd_l_table.h"

#include <cassert>
#include <iostream>
#include "pbd_helper.h"

pbd::l_table::l_table()
  : m_data{}
{
}

pbd::l_table pbd::load_l_table_from_csv(const std::string& csv_filename)
{
  /*

  "","V1","V2","V3","V4","V5","V6"
  "1",1,0,0.2000000001,0.2,-1,1
  "2",2,1,0.2,0.00198989310153758,-1,2

  */
  const std::vector<std::string> text{
    remove_first(
      file_to_vector(csv_filename)
    )
  };

  l_table t;
  if (text.size() <= 1) return t;

  for (const std::string& line: text)
  {
    const auto v = remove_first(seperate_string(line, ','));

    assert(v.size() == 6);
    // - the first column is the incipient-level label of a species
    const int incip_level_lbl{std::stoi(v[0])};
    // - the second column is the incipient-level label of the parent of the species
    const int incip_lvl_lbl_par{std::stoi(v[1])};
    // - the third column is the time at which a species is born as incipient species
    const double t_incipient{std::stod(v[2])};
    // - the fourth column is the time of speciation-completion of the species
    //   If the fourth element equals -1, then the species is still incipient.
    const double t_good{std::stod(v[3])};
    // - the fifth column is the time of extinction of the species
    //   If the fifth element equals -1, then the species is still extant.
    const double t_extinction{std::stod(v[4])};
    // - The sixth column is the species-level label of the species
    const int species_level_label{std::stoi(v[5])};
    t.push_back(
      l_table_row(
        incip_level_lbl,
        incip_lvl_lbl_par,
        t_incipient,
        t_good,
        t_extinction,
        species_level_label
      )
    );
  }
  return t;
}

void pbd::l_table::push_back(const l_table_row& row)
{
  m_data.push_back(row);
}
