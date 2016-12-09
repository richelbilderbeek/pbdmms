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

using namespace std;
using namespace jobo;

jobo::parameters::parameters(
    const int population_size,
    const int seed,
    const double mutation_rate,
    const int n_generations,
    const int loci,
    const double fitness_threshold
    )
  : m_population_size{population_size},
    m_seed{seed},
    m_mutation_rate{mutation_rate},
    m_n_generations{n_generations},
    m_loci{loci},
    m_fitness_threshold{fitness_threshold}
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
  if (m_fitness_threshold < 0)
  {
    throw std::invalid_argument("fitness_threshold must be positive");
  }
}

jobo::parameters jobo::load_parameters(const std::string& filename)
{
  cout << "loads parameters" << '\n';
  //Check if there is a parameter file
  if (!is_regular_file(filename))
  {
    throw std::invalid_argument("parameter file cannot be found");
  }
  std::ifstream f(filename);
  parameters p(3,38,0.5,10,6,0.05);
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
    << p.m_population_size << " "
    << p.m_seed << " "
    << p.m_mutation_rate << " "
    << p.m_n_generations << " "
    << p.m_loci << " "
    << p.m_fitness_threshold
  ;
  return os;
}

std::istream& jobo::operator>>(std::istream& is, parameters& p)
{
  is
    >> p.m_population_size
    >> p.m_seed
    >> p.m_mutation_rate
    >> p.m_n_generations
    >> p.m_loci
    >> p.m_fitness_threshold
  ;
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

