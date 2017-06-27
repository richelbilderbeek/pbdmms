#include "ribi_jkr_adapters.h"

#include "ribi_parameters.h"
#include "ribi_results.h"
#include "pbd_ltt.h"
#include "pbd_nltt.h"
#include <fstream>
#include <iostream>

ribi::ancestry_graph ribi::create_ancestry_graph(const results& /* r */)
{
  return {};
}

ribi::ancestry_graph ribi::create_reconstructed(const ribi::ancestry_graph& g)
{
  //STUB
  return g;
}

ribi::ancestry_graph create_reconstructed(const ribi::ancestry_graph& g)
{
  return ribi::create_reconstructed(g);
}

std::string ribi::get_reconstructed_tree_filename(const parameters& /* p */)
{
  //STUB
  return "ribi_reconstructed.tree";
}
void ribi::save_nltt_plot(const results& r, const std::string& filename)
{
  std::ofstream f(filename);
  f << convert_to_nltt(r.get_ltt());
}

void ribi::save_reconstructed_tree(const ribi::ancestry_graph& g, const std::string& filename)
{
  //STUB
  std::ofstream f(filename);
  f << "STUB: " << boost::num_vertices(g) << '\n';
}

void save_reconstructed_tree(const ribi::ancestry_graph& g, const std::string& filename)
{
  ribi::save_reconstructed_tree(g, filename);
}

void ribi::save_ltt_plot(const results& r, const std::string& filename)
{
  std::ofstream f(filename);
  f << r.get_ltt();
}
