#ifndef KEWE_JKR_ADAPTERS_H
#define KEWE_JKR_ADAPTERS_H

#include "kewe_ancestry_graph.h"
#include "kewe_parameters.h"
#include "kewe_results.h"
#include "kewe_individual.h"
#include "kewe_simulation.h"

namespace kewe {

ancestry_graph create_ancestry_graph(const results& r);
ancestry_graph create_reconstructed(const ancestry_graph& g);

simulation create_simulation(const parameters& p);

individuals create_next_population(
  const simulation& s,
  std::mt19937& gen
);

void run(simulation& s);

results get_results(const simulation& s);

std::string get_nltt_plot_filename(const parameters& p);

std::string get_ltt_plot_filename(const parameters& p);

int  get_n_generations(const parameters& p);

int get_rng_seed(const parameters& p);

void save_ltt_plot(const results& r, const std::string& f);

void save_nltt_plot(const results& r, const std::string& f);

void save_reconstructed_tree(const ancestry_graph& g, const std::string& filename);

void set_population(simulation& s, const individuals& next_pop);

} //~namespace kewe

#endif // KEWE_JKR_ADAPTERS_H
