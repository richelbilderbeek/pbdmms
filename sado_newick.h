#ifndef SADO_NEWICK_H
#define SADO_NEWICK_H

#include <string>
#include <vector>
#include "sado_ancestry_graph.h"

namespace sado {

///Checks if the string is in the Newick format
bool is_newick(const std::string& s);

///Surround s, creating '(s);'
std::string newick_surround(const std::string& s);

///Convert a graph to Newick format
std::string to_newick(const ancestry_graph& g);

///Classic
std::string to_newick_impl1(const ancestry_graph& g);
///New, still fails
std::string to_newick_impl2(const ancestry_graph& g);
///New
std::string to_newick_impl3(const ancestry_graph& g);

///Convert a graph to Newick format
///Easier if the number of generations is known
std::string to_newick(const ancestry_graph& g,
  const int n_generations);

///Recursive function to summarize a vertex (and its ancestors) in a graph to Newick format.
///Will first be called by 'is_newick' with a root node
std::string to_newick(const sp_vert_desc vd, const ancestry_graph& g);

///Recursive function to summarize a vertex (and its ancestors) in a graph to Newick format.
///Will first be called by 'is_newick' with a root node
///Easier if the number of generations is known
std::string to_newick(const sp_vert_desc vd, const ancestry_graph& g,
  const int n_generations);

} //~namespace sado

#endif // SADO_NEWICK_H
