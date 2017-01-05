#include "daic_input_row.h"

///the whitespace is tab
std::string daic::create_input_header() noexcept
{
  return "Clade_name\tStatus\tMissing_species\tBranching_times";
}
