#include "jobo_output.h"
#include "jobo_simulation.h"
#include "jobo_parameters.h"
#include "jobo_individuals.h"
#include "jobo_individual.h"
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
#include <random>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include "count_undirected_graph_connected_components.h"
#include "convert_dot_to_svg.h"
#include "convert_svg_to_png.h"
#include "count_max_number_of_pieces.h"

using namespace std;
using namespace jobo;

vector<int> jobo::run_simulation(
  const jobo::parameters& parameters
)
{
  std::mt19937 rng_engine(parameters.get_seed());
  const double mutation_rate(parameters.get_mutation_rate());
  const int duration(parameters.get_duration());
  vector<individual> individuals (parameters.get_individuals());
  int generations(0);

  // Create vector to store number of good species per generation
  std::vector<int> m_ltt(duration);

  for (int i=0; i!=duration; ++i)
  {
    generations = generations+1;
    assert (generations == i+1);
    individuals = connect_generations(individuals,mutation_rate,rng_engine);

    // Stop simulation if population size is 1
    if (individuals.size() == 1)
    {
      break;
    }
    assert(generations >= 0);

    // Count number of good species for each generation
    int n_good_species = count_good_species(individuals);
    m_ltt[i] = n_good_species;
    int length_m_ltt = static_cast<int>(m_ltt.size());
    assert (length_m_ltt >= 10);
    std::cout << "Generation: " << generations << '\n';
    std::cout << "Number of 'good' species: " << n_good_species << '\n';
    assert (n_good_species > 0);

    /*

    // Count unique genotypes, good species and possible species
    std::vector<genotype> vector_of_genotypes = get_unique_genotypes(individuals);
    int n_unique_genotypes = static_cast<int>(vector_of_genotypes.size());
    //{
    //  std::ofstream f("debug.log");
    //  f << individuals << '\n';
    // }
    //int n_possible_species = count_possible_species(individuals);
    //assert (n_possible_species >= 0);
    //assert (n_possible_species < 25);

    // Show other output
    std::cout << "Number of individuals after extinction: " << individuals.size() << '\n';
    std::cout << "Number of unique genotypes: " << n_unique_genotypes << '\n';
    //std::cout << "Number of 'possible' species: " << n_possible_species << '\n' << '\n';
    */
  }
  return m_ltt;
}

