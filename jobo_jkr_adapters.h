#ifndef JOBO_JKR_ADAPTERS_H
#define JOBO_JKR_ADAPTERS_H

#include <string>
#include "jobo_ancestry_graph.h"
#include "jobo_parameters.h"
#include "jobo_simulation.h"
#include "jobo_results.h"

namespace jobo {

ancestry_graph create_ancestry_graph(const results& r);

ancestry_graph create_reconstructed(const ancestry_graph& g);

simulation create_simulation(const parameters& p);

int get_n_generations(const parameters& p)noexcept;

std::mt19937 get_rng_seed(const parameters& p)noexcept;

individuals create_next_population(const simulation& s, std::mt19937& rng_engine);

void set_population(simulation &s, const individuals& next_population);

std::string get_reconstructed_tree_filename(const parameters& p);

results get_results(const simulation& s);

std::string get_ltt_plot_filename(const parameters& p) noexcept;

std::string get_ltt_plot_viables_filename(const parameters& p) noexcept;

std::string get_ltt_plot_inviables_filename(const parameters& p) noexcept;

std::string get_nltt_plot_filename(const parameters& p) noexcept;

std::string get_nltt_plot_viables_filename(const parameters& p) noexcept;

std::string get_nltt_plot_inviables_filename(const parameters& p) noexcept;

void save_ltt_plot(const results& r, const std::string& filename);

void save_nltt_plot(const results& r, const std::string& filename);

void save_reconstructed_tree(const ancestry_graph& g, const std::string& filename);

void save_ltt_plot_viables(const results& r, const std::string& filename);

void save_ltt_plot_inviables(const results& r, const std::string& filename);

void save_nltt_plot_viables(const results& r, const std::string& filename);

void save_nltt_plot_inviables(const results& r, const std::string& filename);

} //~namespace jobo

jobo::ancestry_graph create_reconstructed(const jobo::ancestry_graph& g);

void save_reconstructed_tree(const jobo::ancestry_graph& g, const std::string& filename);

#endif // JOBO_JKR_ADAPTERS_H














