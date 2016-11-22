#ifndef JOBO_JKR_ADAPTERS_H
#define JOBO_JKR_ADAPTERS_H

#include <string>
#include "jobo_parameters.h"
#include "jobo_simulation.h"
#include "jobo_results.h"

namespace jobo {

simulation create_simulation(const parameters& p);

void run(simulation& s);

jobo_results get_results(const simulation& s);

std::string get_ltt_plot_filename(const parameters& p);

void save_ltt_plot(const jobo_results& r, const std::string& filename);

} //~namespace jobo

#endif // JOBO_JKR_ADAPTERS_H
