#include "daic_run.h"

#include <cassert>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include "daic_helper.h"

daic::output daic::run( //!OCLINT indeed too many parameters. For now, I will accept this
  const input& in,
  const double init_lambda_c,
  const double init_mu,
  const int init_k,
  const double init_gamma,
  const double init_lambda_a,
  const double island_age,
  const int n_species_main,
  const std::string& di_filename,
  const std::string& do_filename,
  const std::string& r_script_filename
)
{
  save(in, di_filename);
  if(!is_regular_file(di_filename))
  {
    std::stringstream msg;
    msg << "DAISIE input file '" << di_filename << "' absent";
    throw std::runtime_error(msg.str().c_str());
  }
  //Create and save R script
  {
    std::ofstream f(r_script_filename);
    f  << create_script_text(
      init_lambda_c,
      init_mu,
      init_k,
      init_gamma,
      init_lambda_a,
      island_age,
      n_species_main,
      di_filename,
      do_filename);
  }
  run_r_script(r_script_filename);

  if(!is_regular_file(do_filename))
  {
    std::stringstream msg;
    msg << "DAISIE output file '" << do_filename << "' absent";
    throw std::runtime_error(msg.str().c_str());
  }
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
  const double island_age,
  const int n_species_main,
  const std::string& di_filename,
  const std::string& do_filename
)
{
  std::stringstream s;
  s
    << "library(DAISIE)" << '\n'
    << "di_filename <- \"" << di_filename << "\"" << '\n'
    << "if(!file.exists(di_filename)) {" << '\n'
    << "  stop(\"Input file '\",di_filename,\"' absent\")" << '\n'
    << "}" << '\n'
    << "df <- read.csv(file = di_filename, sep = '\t')" << '\n'
    << "print(df)" << '\n'
    << "if(nrow(df) == 0) {" << '\n'
    << "  stop(\"Input must have rows\")" << '\n'
    << "}" << '\n'
    << "prepared_df <- DAISIE_dataprep(" << '\n'
    << "  datatable = df," << '\n'
    << "  island_age = "<< island_age << "," << '\n'
    << "  M = " << n_species_main << ")" << '\n'
    << "output <- DAISIE_ML(" << '\n'
    << "   datalist = prepared_df," << '\n'
    << "   initparsopt = c("
      << init_lambda_c << ',' << init_mu << ','
      << init_k << ','
      << init_gamma << ','
      << init_lambda_a << "),"
      << '\n'
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

void daic::set_r_working_directory(
  const std::string& path,
  const std::string& r_script_filename
)
{
  //Create and save script
  {
    std::ofstream f(r_script_filename);
    f << "setwd(\"" << path << "\")" << '\n';
  }
  run_r_script(r_script_filename);
}
