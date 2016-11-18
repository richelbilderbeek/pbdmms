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

//First "results function"  with only cout output
vector<int> jobo::get_m_ltt_good(
    int time,
    const double mutation_rate,
    std::mt19937& rng_engine,
    std::vector<individual>individuals
    )
{
    int generations(0);
    std::vector<int> m_ltt_good(0);
    for (int i=0; i!=time; ++i)
      {
        generations = generations+1;
        individuals = connect_generations(individuals,mutation_rate,rng_engine);
        //Stop simulation if population size is 1
        if (individuals.size() == 1)
        {
          break;
        }
        assert(generations >= 0);

        //Count unique genotypes, good species and possible species
        std::vector<genotype> vector_of_genotypes = get_unique_genotypes(individuals);
        int n_unique_genotypes = static_cast<int>(vector_of_genotypes.size());
        int n_good_species = count_good_species(individuals);
        int n_possible_species = count_possible_species(individuals);

        // Store the numbers of good species for each generation in a vector
        assert (n_good_species != 0);
        m_ltt_good.push_back(n_good_species);
        int sz = static_cast<int>(m_ltt_good.size());
        assert(sz == generations);

        //Show other output
        std::cout << "Generation: " << generations << '\n';
        std::cout << "Number of individuals after extinction: " << individuals.size() << '\n';
        std::cout << "Number of unique genotypes: " << n_unique_genotypes << '\n';
        std::cout << "Number of 'good' species: " << n_good_species << '\n';
        std::cout << "Number of 'possible' species: " << n_possible_species << '\n' <<  '\n';
      }
    return m_ltt_good;;
}
