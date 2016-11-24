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

void jobo::run(simulation& s)
{
  int generations(0);
  const int duration(p.get_duration());
  for (int i=0; i!=duration; ++i)
  {
    do_timestep(parameters);
  }
}

  /*
  parameters p (s.get_parameters());
  //const int population_size (p.get_population_size());
  std::mt19937 rng_engine(p.get_seed());
  const double mutation_rate(p.get_mutation_rate());
  const int duration(p.get_duration());
  vector<individual> individuals (p.get_individuals());
  std::vector<int> m_ltt(duration);
  int generations(0);
  for (int i=0; i!=duration; ++i)
  {
    generations = generations+1;
    individuals = connect_generations(individuals,mutation_rate,rng_engine);
    int n_good_species = count_good_species(individuals);
    m_ltt[i] = n_good_species;
  }
  */

jobo::results jobo::get_results(const simulation& s)
{
  return results(s);
}

std::string jobo::get_ltt_plot_filename(const parameters& p)
{
  std::string filename = create_ltt_plot_filename(p);
  return filename;
}

void jobo::save_ltt_plot(const results& r, const std::string& filename)
{
  std::ofstream file(filename);
  file << r;
}
