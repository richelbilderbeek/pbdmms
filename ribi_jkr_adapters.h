#ifndef RIBI_JKR_ADAPTERS_H
#define RIBI_JKR_ADAPTERS_H

///Functions needed to call 'jkr::do_experiment'
#include <string>

#include "ribi_ancestry_graph.h"
#include "ribi_results.h"
#include "ribi_simulation.h"

namespace ribi {

struct parameters;

ancestry_graph create_ancestry_graph(const results& r);

ancestry_graph create_reconstructed(const ancestry_graph& g);

std::string get_reconstructed_tree_filename(const parameters& p);

void save_nltt_plot(const results& r, const std::string& filename);

void save_ltt_plot(const results& r, const std::string& filename);

void save_reconstructed_tree(const ancestry_graph& g, const std::string& filename);

} //~namespace ribi

ribi::ancestry_graph create_reconstructed(const ribi::ancestry_graph& g);

void save_reconstructed_tree(const ribi::ancestry_graph& g, const std::string& filename);

#endif // RIBI_JKR_ADAPTERS_H
