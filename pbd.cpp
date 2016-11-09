#include "pbd.h"

#include <fstream>

pbd::l_table pbd::sim_to_l_table(
  const double birth_good,
  const double birth_incipient,
  const double completion,
  const double death_good,
  const double death_incipient,
  const double time,
  const int seed
)
{
  const std::string r_filename{"run_pbd_sim.R"};
  const std::string csv_filename{"run_pbd_sim.csv"};
  //Create script to create the phylogenies
  {
    std::ofstream f(r_filename);
    f << "library(PBD)\n"
      << "set.seed("<< seed << ")\n"
      << "output <- PBD::pbd_sim(\n"
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
      << "write.csv(x = output$L, file = \"" << csv_filename << "\")\n"
    ;
  }
  std::system((std::string("Rscript ") + r_filename).c_str());
  return load_l_table_from_csv(csv_filename);
}

pbd::nltt pbd::sim_to_nltt_recon(
  const double birth_good,
  const double birth_incipient,
  const double completion,
  const double death_good,
  const double death_incipient,
  const double time,
  const int seed
)
{
  const std::string r_filename{"sim_to_nltt_recon.R"};
  const std::string csv_filename{"sim_to_nltt_recon.csv"};
  //Create script to create the phylogenies
  {
    std::ofstream f(r_filename);
    f << "library(PBD)\n"
      << "set.seed("<< seed << ")\n"
      << "filename <- \"" << csv_filename << "\"\n"
      << "birth_good <- " << birth_good << "\n"
      << "completion <- " << completion << "\n"
      << "birth_incipient <- " << birth_incipient << "\n"
      << "death_good <- " << death_good << "\n"
      << "death_incipient <- " << death_incipient << "\n"
      << "age  <- " << time << "\n"
      << "out <- PBD::pbd_sim(\n"
      << "  pars = c(\n"
      << "    birth_good,\n"
      << "    completion,\n"
      << "    birth_incipient,\n"
      << "    death_good,\n"
      << "    death_incipient\n"
      << "  ),\n"
      << "  age = age,\n"
      << "  soc = 2\n"
      << ")\n"
      << "\n"
      << "phy <- out$recontree\n"
      << "xy <- ape::ltt.plot.coords( phy, backward = TRUE, tol = 1e-6)\n"
      << "xy[, 2] <- xy[, 2] / max(xy[, 2])\n"
      << "xy[, 1] <- xy[, 1] + abs( min( xy[, 1]))\n"
      << "xy[, 1] <- xy[, 1] / max( xy[, 1])\n"
      << "\n"
      << "write.csv(x = xy, file = filename)\n"
    ;
  }
  std::system((std::string("Rscript ") + r_filename).c_str());
  return load_nltt_from_csv(csv_filename);
}

void pbd::sim_to_png(
  const double birth_good,
  const double birth_incipient,
  const double completion,
  const double death_good,
  const double death_incipient,
  const double time,
  const int seed,
  const std::string& png_filename
)
{
  const std::string r_filename{"run_pbd_sim.R"};
  //Create script to create the phylogenies
  {
    std::ofstream f(r_filename);
    f << "library(PBD)\n"
      << "set.seed("<< seed << ")\n"
      << "png(\"" << png_filename << "\")\n"
      << "output <- PBD::pbd_sim(\n"
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
