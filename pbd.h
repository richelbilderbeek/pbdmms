#ifndef PBD_H
#define PBD_H

#include <string>
#include "pbd_l_table.h"
#include "pbd_nltt.h"
#include "pbd_parameters.h"

///PBD functions
namespace pbd {

///Runs a PBD simulation and returns the incipient species tree as an L-table
/// @param pbd_parameters parameters for a PBD simulation
l_table sim_to_l_table(const parameters& pbd_parameters);

///Runs a PBD simulation and returns the incipient species tree as an L-table
/// @param birth_good probability that a good species
///   produces a new lineage per million year
/// @param birth_incipient probability that an incipient species
///   produces a new lineage per million year
/// @param completion probability that an incipient species
///   becomes a good species per million years
/// @param death_good probability that a good species
/// @param death_incipient good probability that an incipient species
/// @param time simulation tim in million years
/// @param seed random number generator seed
l_table sim_to_l_table(
  const double birth_good,
  const double birth_incipient,
  const double completion,
  const double death_good,
  const double death_incipient,
  const double time,
  const int seed
);

///Runs a PBD simulation and returns the nLTT plot values
/// of the reconstructed tree
/// @param pbd_parameters parameters for a PBD simulation
/// @return a chrononologically ordered (from past to present)
///   collection of {t,N} points, where t is the normalized
///   time (0.0 is crown age, 1.0 is present) and N is the normalized
///   number of lineages (where low values denote few lineages and
///   1.0 denotes the maximum number of lineages). The last point will always
///   be {1.0,1.0}
nltt sim_to_nltt_recon(const parameters& pbd_parameters);

///Runs a PBD simulation and saves the nLTT plot values
/// to a file
/// @param pbd_parameters parameters for a PBD simulation
/// @param csv_filename the filename to save to
/// @return nothing, but save a chrononologically ordered (from past to present)
///   collection of {t,N} points, where t is the normalized
///   time (0.0 is crown age, 1.0 is present) and N is the normalized
///   number of lineages (where low values denote few lineages and
///   1.0 denotes the maximum number of lineages). The last point will always
///   be {1.0,1.0}
void sim_to_nltt_recon(
  const parameters& pbd_parameters,
  const std::string& csv_filename);

///Runs a PBD simulation and returns the nLTT plot values
/// of the complete tree
/// @param pbd_parameters parameters for a PBD simulation
/// @return a chrononologically ordered (from past to present)
///   collection of {t,N} points, where t is the normalized
///   time (0.0 is crown age, 1.0 is present) and N is the normalized
///   number of lineages (where low values denote few lineages and
///   1.0 denotes the maximum number of lineages). The last point will always
///   be {1.0,1.0}
nltt sim_to_nltt_igtree_extinct(const parameters& pbd_parameters);

///Runs a PBD simulation and returns the nLTT plot values
/// of the complete tree
/// @param pbd_parameters parameters for a PBD simulation
/// @param csv_filename name of the file that will contain
///   a chrononologically ordered (from past to present)
///   collection of {t,N} points, where t is the normalized
///   time (0.0 is crown age, 1.0 is present) and N is the normalized
///   number of lineages (where low values denote few lineages and
///   1.0 denotes the maximum number of lineages). The last point will always
///   be {1.0,1.0}
/// @return nothing. A file with name csv_filename is created
void sim_to_nltt_igtree_extinct(
  const parameters& pbd_parameters,
  const std::string& csv_filename);

///Runs a PBD simulation and returns the nLTT plot values
/// of the reconstructed tree
/// @param birth_good probability that a good species
///   produces a new lineage per million year
/// @param birth_incipient probability that an incipient species
///   produces a new lineage per million year
/// @param completion probability that an incipient species
///   becomes a good species per million years
/// @param death_good probability that a good species
/// @param death_incipient good probability that an incipient species
/// @param time simulation tim in million years
/// @param seed random number generator seed
/// @return a chrononologically ordered (from past to present)
///   collection of {t,N} points, where t is the normalized
///   time (0.0 is crown age, 1.0 is present) and N is the normalized
///   number of lineages (where low values denote few lineages and
///   1.0 denotes the maximum number of lineages). The last point will always
///   be {1.0,1.0}
nltt sim_to_nltt_recon(
  const double birth_good,
  const double birth_incipient,
  const double completion,
  const double death_good,
  const double death_incipient,
  const double time,
  const int seed
);

///Runs a PBD simulation and produces a PNG
/// @param pbd_parameters parameters for a PBD simulation
void sim_to_png(
  const parameters& pbd_parameters,
  const std::string& png_filename
);

///Runs a PBD simulation and produces a PNG
/// @param birth_good probability that a good species
///   produces a new lineage per million year
/// @param birth_incipient probability that an incipient species
///   produces a new lineage per million year
/// @param completion probability that an incipient species
///   becomes a good species per million years
/// @param death_good probability that a good species
/// @param death_incipient good probability that an incipient species
/// @param time simulation tim in million years
/// @param seed random number generator seed
/// @param png_filename name of the produced PNG file
void sim_to_png(
  const double birth_good,
  const double birth_incipient,
  const double completion,
  const double death_good,
  const double death_incipient,
  const double time,
  const int seed,
  const std::string& png_filename
);

} //~namespace pbd

#endif // PBD_H
