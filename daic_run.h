#ifndef DAIC_RUN_H
#define DAIC_RUN_H

#include "daic_input.h"
#include "daic_output.h"

namespace daic {

/// @param di_filename DAISIE input filename, the temporary file used to save the DAISIE input
/// @param do_filename DAISIE output filename, the temporary file used to save the DAISIE output
std::string create_script_text(
  const double init_lambda_c = 2.5,
  const double init_mu = 2.7,
  const int init_k = 20,
  const double init_gamma = 0.009,
  const double init_lambda_a = 1.01,
  const std::string& di_filename = "daisie_input.csv",
  const std::string& do_filename = "daisie_output.csv"
);

///Run DAISEE
/// @param di_filename DAISIE input filename, the temporary file used to save the DAISIE input
/// @param do_filename DAISIE output filename, the temporary file used to save the DAISIE output
/// @param r_script_filename temporary file to store the R script used to run DAISIE
output run(
  const input& in,
  const double init_lambda_c = 2.5,
  const double init_mu = 2.7,
  const int init_k = 20,
  const double init_gamma = 0.009,
  const double init_lambda_a = 1.01,
  const std::string& di_filename = "daisie_input.csv",
  const std::string& do_filename = "daisie_output.csv",
  const std::string& r_script_filename = "run_daisie.r"
);

void run_r_script(const std::string& r_script_filename);

} //~namespace daic


#endif // DAIC_RUN_H
