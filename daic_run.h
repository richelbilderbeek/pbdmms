#ifndef DAIC_RUN_H
#define DAIC_RUN_H

#include "daic_input.h"
#include "daic_output.h"

namespace daic {

///Run DAISEE
/// @param daisie_input_filename temporary file used to save the DAISIE input
/// @param daisie_output_filename temporary file used to save the DAISIE output
/// @param r_script_filename temporary file to store the R script used to run DAISIE
output run(
  const input& in,
  const std::string& daisie_input_filename = "daisie_input.csv",
  const std::string& daisie_output_filename = "daisie_output.csv",
  const std::string& r_script_filename = "run_daisie.r"
);

} //~namespace daic


#endif // DAIC_RUN_H
