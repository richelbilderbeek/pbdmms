#ifndef JOBO_JKR_ADAPTERS_H
#define JOBO_JKR_ADAPTERS_H

#include <string>
#include "jobo_parameters.h"
#include "jobo_simulation.h"
#include "jobo_results.h"

namespace jobo {

simulation create_simulation(const parameters& p);

int get_n_generations(const parameters& p)noexcept;

std::mt19937 get_rng_seed(const parameters& p)noexcept;

const auto create_next_population (simulation& s);

const auto set_population(simulation& s, vector<individual> next_population);

void run(simulation& s);

results get_results(const simulation& s);

std::string get_ltt_plot_filename(const parameters& p) noexcept;

void save_ltt_plot(const results& r, const std::string& filename);

} //~namespace jobo

#endif // JOBO_JKR_ADAPTERS_H
