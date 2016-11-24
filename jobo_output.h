#ifndef JOBO_OUTPUT_H
#define JOBO_OUTPUT_H

#include <set>
#include <random>
#include "jobo_parameters.h"
#include "jobo_individuals.h"
#include "jobo_individual.h"
#include "jobo_simulation.h"
#include "jobo_results.h"
#include <stdexcept>

using namespace std;

namespace jobo {

///Run simulation and obtains a vector of nLTT values of good species
//vector<int> run_simulation(const parameters& parameters);

///Create filename for ltt_plot
///std::string create_ltt_plot_filename(const parameters& parameters);

} //~namespace jobo
#endif // JOBO_OUTPUT_H
