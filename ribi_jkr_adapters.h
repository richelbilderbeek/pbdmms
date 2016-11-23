#ifndef RIBI_JKR_ADAPTERS_H
#define RIBI_JKR_ADAPTERS_H

///Functions needed to call 'jkr::do_experiment'

#include "ribi_results.h"
#include "ribi_simulation.h"
#include <string>

namespace ribi {

struct parameters;

simulation create_simulation(const parameters& p) noexcept;

std::string get_ltt_plot_filename(const parameters& p) noexcept;
int get_n_generations(const parameters& p) noexcept;

results get_results(const simulation& s) noexcept;
void run(simulation& s);
void save_ltt_plot(const results& r, const std::string& filename);

} //~namespace ribi

#endif // RIBI_JKR_ADAPTERS_H
