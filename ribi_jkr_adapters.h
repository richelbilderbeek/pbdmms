#ifndef RIBI_JKR_ADAPTERS_H
#define RIBI_JKR_ADAPTERS_H

///Functions needed to call 'jkr::do_experiment'

#include "ribi_ancestry_graph.h"
#include "ribi_results.h"
#include "ribi_simulation.h"
#include <string>

namespace ribi {

struct parameters;

ancestry_graph create_ancestry_graph(const results& r);

ancestry_graph create_reconstructed(const ancestry_graph& g);

void save_nltt_plot(const results& r, const std::string& filename);

void save_ltt_plot(const results& r, const std::string& filename);

void save_reconstructed_tree(const ancestry_graph& g, const std::string& filename);

} //~namespace ribi

#endif // RIBI_JKR_ADAPTERS_H
