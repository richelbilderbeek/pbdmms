#ifndef PBD_H
#define PBD_H

#include <string>

///PBD functions
namespace pbd {

///Runs a PBD simulation,
///saves the phylogenies to a file
void run_pbd_sim(
  const double birth_good,
  const double birth_incipient,
  const double completion,
  const double death_good,
  const double death_incipient,
  const double time,
  const int seed,
  const std::string& filename
);

} //~namespace pbd

#endif // PBD_H
