#include "jobo_jkr_adapters.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
#include <random>
#include <cstdio>
#include <cctype>
#include <string>
#include <stdexcept>
#include <sstream>
#include <random>
#include <gsl/gsl_assert>

jobo::ancestry_graph jobo::create_ancestry_graph(const results& /* r */)
{
  return {};
}

jobo::ancestry_graph jobo::create_reconstructed(const jobo::ancestry_graph& g)
{
  //STUB
  return g;
}

jobo::ancestry_graph create_reconstructed(const jobo::ancestry_graph& g)
{
  return jobo::create_reconstructed(g);
}

jobo::simulation jobo::create_simulation(const parameters& p)
{
  return simulation(p);
}

int jobo::get_n_generations(const parameters& p) noexcept
{
 return p.get_generations();
}

std::mt19937 jobo::get_rng_seed(const parameters& p) noexcept
{
  std::mt19937 rng_engine(p.get_seed());
  return rng_engine;
}

jobo::individuals jobo::create_next_population(const simulation& s, std::mt19937& rng_engine)
{
  return create_next_generation(
      s.get_individuals(),
      s.get_parameters(),
      rng_engine
  );
}

void jobo::set_population(simulation& s, const individuals& next_population)
{
  #ifndef NDEBUG
  const int n_viables_before{static_cast<int>(get_results(s).get_ltt_viables().size())};
  const int n_inviables_before{static_cast<int>(get_results(s).get_ltt_inviables().size())};
  #endif //NDEBUG

  //Measure current generation (may be the initial population)
  vector<genotype> viable_population = collect_viable_genotypes(s.get_individuals());
  assert(viable_population.size()>0);
  //std::cout << viable_population.size() << '\n';
  const int n_viable_species = count_good_species(viable_population);
  //std::cout << n_viable_species << '\n';
  const std::vector<genotype> inviable_population = get_unique_genotypes(next_population);
  const int n_invia_species = count_good_species(inviable_population);
  s.get_results().add_ltt_viable(n_viable_species);
  s.get_results().add_ltt_inviable(n_invia_species);

  s.set_individuals(next_population);

  #ifndef NDEBUG
  const int n_viables_after{static_cast<int>(get_results(s).get_ltt_viables().size())};
  const int n_inviables_after{static_cast<int>(get_results(s).get_ltt_inviables().size())};
  Ensures(n_viables_after > n_viables_before);
  Ensures(n_inviables_after > n_inviables_before);
  #endif //NDEBUG

}

jobo::results jobo::get_results(const simulation& s)
{
  //Get results from m_results in jobo::simulation to jobo::results
  return s.get_results();
}

std::string jobo::get_ltt_plot_filename(const parameters& p) noexcept
{
  return get_ltt_plot_viables_filename(p);
}

std::string jobo::get_ltt_plot_viables_filename(const parameters& p) noexcept
{
  return p.get_ltt_plot_filename_vi();
}

std::string jobo::get_ltt_plot_inviables_filename(const parameters& p) noexcept
{
  return p.get_ltt_plot_filename_in();
}

std::string jobo::get_nltt_plot_filename(const parameters& p) noexcept
{
  return get_nltt_plot_viables_filename(p);
}

std::string jobo::get_nltt_plot_viables_filename(const parameters& p) noexcept
{
  return p.get_nltt_plot_filename_v();
}

std::string jobo::get_nltt_plot_inviables_filename(const parameters& p) noexcept
{
  return p.get_nltt_plot_filename_i();
}

std::string jobo::get_reconstructed_tree_filename(const parameters& /* p */)
{
  //STUB
  return "jobo_reconstructed.tree";
}

void jobo::save_nltt_plot(const results& r, const std::string& filename)
{
  save_nltt_plot_viables(r,filename);
}

void jobo::save_nltt_plot_inviables(const results& r, const std::string& filename)
{
  std::ofstream file(filename);
  file << r.get_nltt_inviables();
}

void jobo::save_nltt_plot_viables(const results& r, const std::string& filename)
{
  std::ofstream file(filename);
  file << r.get_nltt_viables();
}

void jobo::save_ltt_plot(const results& r, const std::string& filename)
{
  save_ltt_plot_viables(r, filename);
}

void jobo::save_ltt_plot_viables(const results& r, const std::string& filename)
{
  std::ofstream file(filename);
  for (const auto i: r.get_ltt_viables())
  {
    file << i << ',';
  }
  //file << r.get_ltt_viables()
}

void jobo::save_ltt_plot_inviables(const results& r, const std::string& filename)
{
  std::ofstream file(filename);
  for (const auto i: r.get_ltt_inviables())
  {
    file << i << ',';
  }
  //file << r.get_ltt_inviables()
}

void jobo::save_reconstructed_tree(const jobo::ancestry_graph& g, const std::string& filename)
{
  //STUB
  std::ofstream f(filename);
  f << "STUB: " << boost::num_vertices(g) << '\n';
}

void save_reconstructed_tree(const jobo::ancestry_graph& g, const std::string& filename)
{
  jobo::save_reconstructed_tree(g, filename);
}






