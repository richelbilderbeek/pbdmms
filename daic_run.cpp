#include "daic_run.h"

#include <fstream>
#include <sstream>
#include <iostream>

daic::output daic::run(
  const input& in,
  const double init_lambda_c,
  const double init_mu,
  const int init_k,
  const double init_gamma,
  const double init_lambda_a,
  const std::string& di_filename,
  const std::string& do_filename,
  const std::string& r_script_filename
)
{
  save(in, di_filename);
  {
    std::ofstream f(r_script_filename);
    f  << create_script_text(init_lambda_c,init_mu,init_k,init_gamma,init_lambda_a, di_filename, do_filename);
  }
  run_r_script(r_script_filename);
  return read_output_from_file(do_filename);
}

void daic::run_r_script(const std::string& r_script_filename)
{
  std::stringstream cmd;
  cmd << "Rscript " << r_script_filename;
  const int error_number{
    std::system(cmd.str().c_str())
  };
  if (!error_number)
  {
    std::cerr << "run_r_script has finished with error " << error_number << '\n';
  }
}

std::string daic::create_script_text(
  const double init_lambda_c,
  const double init_mu,
  const int init_k,
  const double init_gamma,
  const double init_lambda_a,
  const std::string& di_filename,
  const std::string& do_filename
)
{
  std::stringstream s;
  s
    << "library(DAISIE)" << '\n'
    << "df <- read.csv(file = \"" << di_filename << "\", sep = '\t')" << '\n'
    << "prepared_df <- DAISIE_dataprep(" << '\n'
    << "  datatable = df," << '\n'
    << "  island_age = 4," << '\n'
    << "  M = 1000)" << '\n'
    << "output <- DAISIE_ML(" << '\n'
    << "   datalist = prepared_df," << '\n'
    << "   initparsopt = c(" << init_lambda_c << ',' << init_mu << ',' << init_k << ',' << init_gamma << ',' << init_lambda_a << ")," << '\n'
    << "   ddmodel = 11," << '\n'
    << "   idparsopt = 1:5," << '\n'
    << "   parsfix = NULL," << '\n'
    << "   idparsfix = NULL" << '\n'
    << ")" << '\n'
    << "write.csv(" << '\n'
    << "  x = output," << '\n'
    << "  file = \"" << do_filename << "\"," << '\n'
    << "  row.names = FALSE" << '\n'
    << ")" << '\n'
  ;
  return s.str();
}
