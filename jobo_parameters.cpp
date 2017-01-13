#include "jobo_parameters.h"
#include "jobo_individuals.h"
#include "jobo_individual.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cassert>
#include <fstream>
#include <algorithm>
#include <set>
#include <cstdio>
#include <cctype>
#include <string>
#include <random>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include "is_regular_file.h"

jobo::parameters::parameters(
    const int population_size,
    const int seed,
    const double mutation_rate,
    const int n_generations,
    const int loci,
    const std::string& ltt_plot_filename_vi,
    const std::string& ltt_plot_filename_in
  )
  : m_population_size{population_size},
    m_seed{seed},
    m_mutation_rate{mutation_rate},
    m_n_generations{n_generations},
    m_loci{loci},
    m_ltt_plot_filename_vi{ltt_plot_filename_vi},
    m_ltt_plot_filename_in{ltt_plot_filename_in}
{
  // Give exceptions for impossible parameter settings
  if (m_population_size < 2)
  {
    throw std::invalid_argument("population_size must be 2 or larger");
  }
  if (m_mutation_rate < 0)
  {
    throw std::invalid_argument("mutation_rate must be positive");
  }
  if (m_mutation_rate > 1)
  {
    throw std::invalid_argument("mutation_rate can't be higher than 1");
  }
  if (m_n_generations < 0)
  {
    throw std::invalid_argument("n_generations must be positive");
  }
  if (m_loci < 2)
  {
    throw std::invalid_argument("number of loci must be 2 or larger");
  }
  assert(!m_ltt_plot_filename_vi.empty());
  assert(!m_ltt_plot_filename_in.empty());
}

jobo::parameters jobo::create_test_parameters_1() noexcept
{
  const int population_size{100};
  const int seed{42};
  const double mutation_rate{0.1};
  const int n_generations{100};
  const int n_loci{20};
  return parameters(
    population_size,
    seed,
    mutation_rate,
    n_generations,
    n_loci
  );
}

jobo::parameters jobo::create_profiling_parameters() noexcept
{
  const int population_size{100};
  const int seed{42};
  const double mutation_rate{0.1};
  const int n_generations{100};
  const int n_loci{32};
  return parameters(
    population_size,
    seed,
    mutation_rate,
    n_generations,
    n_loci
  );
}

jobo::parameters jobo::load_parameters(const std::string& filename)
{
  //Check if there is a parameter file
  if (!is_regular_file(filename))
  {
    throw std::invalid_argument("parameter file cannot be found");
  }
  std::ifstream f(filename);
  parameters p = create_test_parameters_1(); //Just any, will be overwritten anyways
  f >> p;
  return p;
}

void jobo::save_parameters(
  const parameters& p,
  const std::string& filename
)
{
  std::ofstream f(filename);
  f << p;
}

std::ostream& jobo::operator<<(std::ostream& os, const parameters& p)
{
  os
    << "population_size: " << p.m_population_size << '\n'
    << "seed: " << p.m_seed << '\n'
    << "mutation_rate: " << p.m_mutation_rate << '\n'
    << "n_generations: " << p.m_n_generations << '\n'
    << "loci: " << p.m_loci << '\n'
  ;
  return os;
}

std::istream& jobo::operator>>(std::istream& is, parameters& p)
{
  std::string population_size_name;
  std::string seed_name;
  std::string mutation_rate_name;
  std::string n_generations_name;
  std::string loci_name;
  is
    >> population_size_name
    >> p.m_population_size
    >> seed_name
    >> p.m_seed
    >> mutation_rate_name
    >> p.m_mutation_rate
    >> n_generations_name
    >> p.m_n_generations
    >> loci_name
    >> p.m_loci
  ;
  assert(population_size_name == "population_size:");
  assert(seed_name == "seed:");
  assert(mutation_rate_name == "mutation_rate:");
  assert(n_generations_name == "n_generations:");
  assert(loci_name == "loci:");

  return is;
}

bool jobo::operator==(const parameters& lhs, const parameters& rhs) noexcept
{
  return lhs.get_population_size() == rhs.get_population_size();
}

bool jobo::operator!=(const parameters& lhs, const parameters& rhs) noexcept
{
  return !(lhs == rhs);
}

