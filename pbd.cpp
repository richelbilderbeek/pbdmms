#include "pbd.h"

#include <fstream>

void pbd::run_pbd_sim(
  const double birth_good,
  const double birth_incipient,
  const double completion,
  const double death_good,
  const double death_incipient,
  const double time,
  const int seed,
  const std::string& filename
)
{
  const std::string r_filename{"run_pbd_sim.R"};
  //Create script to create the phylogenies
  {
    std::ofstream f(r_filename);
    f << "library(PBD)\n"
      << "set.seed("<< seed << ")\n"
      << "png(\"" << filename << "\")\n"
      << "PBD::pbd_sim(\n"
      << "  pars = c(\n"
      << "    " << birth_good << ",\n"
      << "    " << completion << ",\n"
      << "    " << birth_incipient << ",\n"
      << "    " << death_good << ",\n"
      << "    " << death_incipient << "\n"
      << "  ),\n"
      << "  age = " << time << ",\n"
      << "  soc = 2,\n"
      << "  plotit = TRUE\n"
      << ")\n"
      << "dev.off()\n"
    ;
  }
  std::system((std::string("Rscript ") + r_filename).c_str());
}
