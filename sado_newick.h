#ifndef SADO_NEWICK_H
#define SADO_NEWICK_H

#include <string>
#include <vector>
#include "sado_species_graph.h"

namespace sado {

///Checks if the string is in the Newick format
bool is_newick(const std::string& s);

///Convert a graph to Newick format
std::string to_newick(const species_graph& g);

} //~namespace sado

#endif // SADO_NEWICK_H
