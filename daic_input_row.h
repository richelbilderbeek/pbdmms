#ifndef DAIC_INPUT_ROW_H
#define DAIC_INPUT_ROW_H

#include <iosfwd>
#include <string>
#include <vector>
#include "daic_species_status.h"
namespace daic {

class input_row
{
public:
  input_row(
    const std::string& clade_name,
    const species_status status,
    const int n_missing_species,
    const std::vector<double>& branching_times
  );

private:
  std::string& m_clade_name;
  species_status m_status;
  int m_n_missing_species;
  std::vector<double> m_branching_times;
};

std::ostream& operator<<(std::ostream& os, const input_row& r) noexcept;

} //~namespace daic

#endif // DAIC_INPUT_ROW_H
