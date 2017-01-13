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
  //Measure current generation (may be the initial population)
  vector<genotype> viable_population = remove_inviable_species(s.get_individuals());
  assert(viable_population.size()>0);
  const int n_viable_good_species = count_good_species(viable_population);
  const std::vector<genotype> inviable_population = get_unique_genotypes(next_population);
  const int n_inviable_good_species = count_good_species(inviable_population);
  s.get_results().add_ltt_viable(n_viable_good_species);
  s.get_results().add_ltt_inviable(n_inviable_good_species);

  s.set_individuals(next_population);
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
  return p.get_ltt_plot_filename_viables();
}

std::string jobo::get_ltt_plot_inviables_filename(const parameters& p) noexcept
{
  return p.get_ltt_plot_filename_inviables();
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
