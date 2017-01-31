#ifndef RIBI_JKR_ADAPTERS_H
#define RIBI_JKR_ADAPTERS_H

///Functions needed to call 'jkr::do_experiment'

#include "ribi_results.h"
#include "ribi_simulation.h"
#include <string>

namespace ribi {

struct parameters;

void save_ltt_plot(const results& r, const std::string& filename);

} //~namespace ribi

#endif // RIBI_JKR_ADAPTERS_H
