#ifndef SADO_NEWICK_H
#define SADO_NEWICK_H

#include <string>
#include <vector>
#include "sado_species_graph.h"

namespace sado {

///Checks if the string is in the Newick format
bool is_newick(const std::string& s);

///Surround s, creating '(s);'
std::string newick_surround(const std::string& s);

///Convert a graph to Newick format
std::string to_newick(const ancestry_graph& g);

///Recursive function to summarize a vertex (and its ancestors) in a graph to Newick format.
///Will first be called by 'is_newick' with a root node
std::string to_newick(const sp_vert_desc vd, const ancestry_graph& g);

} //~namespace sado

#endif // SADO_NEWICK_H
