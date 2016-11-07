#ifndef PBD_H
#define PBD_H

#include <string>
#include "pbd_l_table.h"

///PBD functions
namespace pbd {

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
l_table run_pbd_sim(
  const double birth_good,
  const double birth_incipient,
  const double completion,
  const double death_good,
  const double death_incipient,
  const double time,
  const int seed
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
void run_pbd_sim(
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
