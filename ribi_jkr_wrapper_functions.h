#ifndef RIBI_JKR_WRAPPER_FUNCTIONS_H
#define RIBI_JKR_WRAPPER_FUNCTIONS_H

#include "ribi_simulation.h"
#include <iosfwd>

namespace ribi {

struct parameters;
struct results;

simulation create_simulation(const parameters& p);
results get_results(const simulation& s);
void run(const simulation& s);
results get_results(const simulation& s);
void save_nll_plot(const results& r, const std::string& filename);
void save_phylogeny(const results& r, const std::string& filename);


} //~namespace ribi

#endif // RIBI_JKR_WRAPPER_FUNCTIONS_H
