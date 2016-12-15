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
  return connect_generations(
    s.get_individuals(),
    s.get_parameters().get_mutation_rate(),
    s.get_parameters().get_fitness_threshold(),
    rng_engine
  );
}

void jobo::set_population(simulation& s, const individuals& next_population)
{
  //Measure current generation (may be the initial population)
  const int n_good_species = count_good_species(s.get_individuals());
  s.get_results().add_ltt(n_good_species);

  s.set_individuals(next_population);
}

jobo::results jobo::get_results(const simulation& s)
{
  //Get results from m_results in jobo::simulation to jobo::results
  return s.get_results();
}

std::string jobo::get_ltt_plot_filename(const parameters& p) noexcept
{
  cout << " filename is made" << '\n';
  const int population_size (p.get_population_size());
  const int seed(p.get_seed());
  const double mutation_rate(p.get_mutation_rate());
  const int n_generations(p.get_generations());
  const int loci (p.get_n_loci());
  const double fitness_threshold (p.get_fitness_threshold());
  std::string genotype (create_initial_genotype(loci));
  std::stringstream s;
  s << "jobo "  << genotype << ',' << population_size << ',' << seed  << ','
                << mutation_rate   << ',' << n_generations << ','
                << loci << ',' << fitness_threshold;
  cout << s.str() << '\n';
  return s.str();
  //alternatively: return "jobo_ltt.csv";
}

void jobo::save_ltt_plot(const results& r, const std::string& filename)
{
  cout << " filename is saved" << '\n';
  std::ofstream file(filename);
  file << r;
}
