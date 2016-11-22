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

jobo::simulation::simulation(
  const parameters& parameters
) noexcept
  : m_individuals(parameters.get_population_size()),
    m_parameters{parameters},
    m_rng_engine(parameters.get_seed())
{
}

std::vector<int> jobo::get_random_ints(std::mt19937& rng_engine, int n)
{
  // Use number of loci to get number of random ints with 1 seed
  std::vector<int> n_loci_ints;
  n_loci_ints.resize(n);
  std::uniform_int_distribution<int> distribution(0,100);
  for (int i=0; i!=n; ++i)
  {
    int w = distribution(rng_engine);
    n_loci_ints[i] =  w;
  }
  // Return all random ints in one vector
  return n_loci_ints;
}

std::vector<double> jobo::get_random_doubles(std::mt19937& rng_engine, int n)
{
  // Use number of loci to get number of random doubles with 1 seed
  std::vector<double> n_loci_doubles;
  n_loci_doubles.resize(n);

  std::uniform_real_distribution<double> distribution(0,1);
  for (int i=0; i!=n; ++i)
  {
    double w = distribution(rng_engine);
    n_loci_doubles[i] =  w;
  }
  // Return all random doubles in one vector
  return n_loci_doubles;
}

std::vector<int> jobo::get_random_parents(
  std::mt19937& rng_engine,
  int population_size
)
{
  std::vector<int> random_parents;
  const int number_of_parents{200};

  // TODO Parents can't be one and the same!
  //bool parents_similar = false;
  //const int n_couples{static_cast<int>(number_of_parents / 2)};
  //const int number_of_parents = population_size*2;

  //do
  //{
      random_parents.resize(number_of_parents);
      std::uniform_int_distribution<int> distribution(0,population_size-1);
      for (int i=0; i!=number_of_parents; ++i)
      {
        int w = distribution(rng_engine);
        random_parents[i] =  w;
      }

      /*//bool parents_similar = false;
      for (int i=0; i!=n_couples; ++i)
      {
        if(random_parents[i] == random_parents[i+n_couples])
        {
          parents_similar = true; break;
        }
      }
     }
  while(parents_similar);
  */
  return random_parents;
}

std::vector<individual> jobo::goto_next_generation(
  std::vector<individual> individuals,
  const double mutation_rate,
  std::mt19937& rng_engine
)
{
  const int population_size{static_cast<int>(individuals.size())};

  // Get random numbers to select random individuals
  const std::vector<int> random_parents = get_random_parents(rng_engine, population_size);
  const int n_couples{static_cast<int>(random_parents.size()) / 2};
  std::vector<individual> new_individuals;

  // Repeat create_offspring by the number of couples
  for (int i=0; i!=n_couples; ++i)
  {
    // Get random father, pick random individual from vector
    const int number_father = random_parents[i];
    const individual father = individuals[number_father];
    // Get random mother, pick random individual from vector
    const int number_mother = random_parents[i+n_couples];
    const individual mother = individuals[number_mother];
    // Create kid
    const individual offspring = create_offspring(mother, father, rng_engine);
    if (offspring.get_genotype().size() % 2 != 0)
    {
      throw std::invalid_argument("genotype length must be even");
    }
    new_individuals.push_back(offspring);
  }

  // After the recombination step the incompatible individuals die
  new_individuals = extinction_low_fitness(new_individuals);

  // Loop through every individual of new_individuals to check for mutation(s)
  for (int i=0; i!=population_size; ++i)
  {
    // Use create_mutation for genotype of each individual
    new_individuals[i] = create_mutation(new_individuals[i],mutation_rate,rng_engine);
  }
  const int n_new_individuals{static_cast<int>(new_individuals.size())};
  assert(n_new_individuals <= 100);
  assert(n_new_individuals >= 1);
  return new_individuals;
}

std::vector<individual> jobo::extinction_low_fitness(
  std::vector<individual> new_individuals
)
{
  // Loop through every individual of new_individuals to check fitness level
  const int population_size{static_cast<int>(new_individuals.size())};
  std::vector<int> fitness_levels;
  std::vector<individual> living_individuals;
  living_individuals = new_individuals;

  for (int i=0; i!=population_size; ++i)
  {
    const individual k = new_individuals[i];
    // Use calc_fitness to get fitness level of each genotype
    assert(k.get_genotype().size() % 2 == 0);
    int n_low_fitness = calc_fitness(k.get_genotype());
    // Make vector of fitness levels for each (new)individual
    fitness_levels.push_back(n_low_fitness);
  }

  // Use fitness vector to remove individual(s) from new_individuals
  const int f{static_cast<int>(fitness_levels.size()-1)};
  for (int i=f; i!=-1; --i)
  {
    if (fitness_levels[i] != 1)
    {
      living_individuals.erase(living_individuals.begin()+i);
      fitness_levels.erase(fitness_levels.begin()+i);
    }
  }

  const int g{static_cast<int>(fitness_levels.size()-1)};
  for (int i=g; i!=-1; --i)
  {
    assert(fitness_levels[i] != 0);
  }

  const int living_size{static_cast<int>(living_individuals.size())};
  for (int i=0; i!=living_size; ++i)
    {
      const individual w = living_individuals[i];
      assert(w.get_genotype() != "ABCDEF");
    }
  return living_individuals;
}

std::vector<individual> jobo::connect_generations(
    std::vector<individual> individuals,
    const double mutation_rate,
    std::mt19937& rng_engine
)
{
  // Make circle complete with goto_next_generation
  std::vector<individual> new_individuals = goto_next_generation(
    individuals,mutation_rate,rng_engine);
  std::vector<individual> living_individuals = extinction_low_fitness(new_individuals);

  // Translate living_individuals into individuals
  individuals = living_individuals;
  new_individuals = living_individuals;
  return individuals;
}

std::vector<genotype> jobo::get_unique_genotypes(
    std::vector<individual> individuals
)
{
  const int population_size{static_cast<int>(individuals.size())};
  // Run through population to collect all genotypes
  set<std::string> set_of_genotypes;
  for (int i=0; i!=population_size; ++i)
  {
    // Create set function to store all unique genotypes, always one or more
    const individual w = individuals[i];
    set_of_genotypes.insert(w.get_genotype());
  }
  // Change set_of_genotypes into vector_of_genotypes
  vector<string>vector_of_genotypes(
    set_of_genotypes.begin(),
    set_of_genotypes.end()
  );

  // Return set with all unique genotypes
  return vector_of_genotypes;
}

double jobo::calc_chance_dead_kids(
    genotype w, genotype q
)
{
  // Test if both genotypes have same size
  assert(w.size() == q.size());
  const int wz{static_cast<int>(w.size())};
  vector<double> chs_dead_offspring;
  for (int i=0; i!=wz; i+=2)
  {
    // Test if both first loci are lower case letters = 0
    double ch_dead_offspring;
    if(w[i] == q[i])
    {
      ch_dead_offspring = 0;
    }
    // Test if both second loci are lower case letters = 0
    else if (w[i+1] == q[i+1])
    {
      ch_dead_offspring = 0;
    }
    // In other cases the chance to die for the loci couple = 0,25
    else
    ch_dead_offspring = 0.25;
   chs_dead_offspring.push_back(ch_dead_offspring);
  }

  // Calculate the chance of dead offspring for all loci together
  double chance_dead_kids = 0;
  std::for_each(chs_dead_offspring.begin(), chs_dead_offspring.end(),
  [&] (double n) {
    chance_dead_kids += n;
  });
  chance_dead_kids = chance_dead_kids/(w.size()/2);

  return chance_dead_kids;
}

int jobo::count_good_species(std::vector<individual> individuals)
{
  if (individuals.empty()) return 0;
  // Ditch the duplicates to speed up the calculation
  const std::vector<genotype> z = get_unique_genotypes(individuals);
  assert(z.size()>0);
  assert(z.size()<100);
  const int sz{static_cast<int>(z.size())};
  if (sz == 1) return 1;
  boost::adjacency_list<
    boost::vecS, boost::vecS, boost::undirectedS, std::string
  > g;
  for (const auto genotype: z)
  {
    boost::add_vertex(genotype, g);
  }
  for (int i=0; i!=sz; ++i)
  {
    for (int j=i+1; j!=sz; ++j)
    {
      const double p{calc_chance_dead_kids(z[i], z[j])};
      if (p < 0.001)
      {
        const auto vip = vertices(g);
        auto from_iter = vip.first + i;
        auto to_iter = vip.first + j;
        boost::add_edge(*from_iter, *to_iter, g);
      }
    }
  }
  {
    /*
    // Create picture of all genotypes and their connections
    const std::string dot_filename{"jobo_count_good_species.dot"};
    const std::string svg_filename{"jobo_count_good_species.svg"};
    const std::string png_filename{"jobo_count_good_species.png"};
    std::ofstream f(dot_filename);
    boost::write_graphviz(f, g,
      [g](std::ostream& os, const auto iter)
      {
        os << "[label=\"" << g[iter] << "\"]";
      }
    );
    f.close();
    convert_dot_to_svg(dot_filename, svg_filename);
    convert_svg_to_png(svg_filename, png_filename);
    std::system("display jobo_count_good_species.png");
    */
  }
  return count_undirected_graph_connected_components(g);
}

int jobo::count_possible_species(std::vector<individual> individuals)
{
  if (individuals.empty()) return 0;
  // Ditch the duplicates to speed up the calculation
  const std::vector<genotype> z = get_unique_genotypes(individuals);
  assert(z.size()>0);
  assert(z.size()<100);
  const int sz{static_cast<int>(z.size())};
  if (sz == 1) return 0;
  boost::adjacency_list<
    boost::vecS, boost::vecS, boost::undirectedS, std::string
  > g;
  for (const auto genotype: z)
  {
    boost::add_vertex(genotype, g);
  }
  for (int i=0; i!=sz; ++i)
  {
    for (int j=i+1; j!=sz; ++j)
    {
      const double p{calc_chance_dead_kids(z[i], z[j])};
      if (p < 0.001)
      {
        const auto vip = vertices(g);
        auto from_iter = vip.first + i;
        auto to_iter = vip.first + j;
        boost::add_edge(*from_iter, *to_iter, g);
      }
    }
  }
  {
    /*
    // Create picture of all genotypes and their connections
    const std::string dot_filename{"jobo_count_possible_species.dot"};
    const std::string svg_filename{"jobo_count_possible_species.svg"};
    const std::string png_filename{"jobo_count_possible_species.png"};
    std::ofstream f(dot_filename);
    boost::write_graphviz(f, g,
      [g](std::ostream& os, const auto iter)
      {
        os << "[label=\"" << g[iter] << "\"]";
      }
    );
    f.close();
    convert_dot_to_svg(dot_filename, svg_filename);
    convert_svg_to_png(svg_filename, png_filename);
    std::system("display jobo_count_possible_species.png");
    */
  }
  return count_max_number_of_pieces(g);
}

// Note:
// It's not about how many genotypes you can shoot,
// It's about the maximum number of species you can achieve by shooting genotypes

// Create test population for tests
std::vector<genotype> jobo::create_test_population_1(
  int time
)
{
  const double mutation_rate (0.5);
  int generations (0);
  mt19937 rng_engine(42);
  vector<individual> individuals(100, individual("abcdef"));
  for (int i=0; i!=time; ++i)
  {
     individuals = connect_generations(individuals,mutation_rate,rng_engine);
     const int n_individuals{static_cast<int>(individuals.size())};
     assert (n_individuals >= 1);
     generations = generations+i;
  }
  vector<genotype> vector_of_genotypes = get_unique_genotypes(individuals);
  const int n_genotypes{static_cast<int>(vector_of_genotypes.size())};
  assert (n_genotypes >= 1);
  return vector_of_genotypes;
}

// Check if vector of genotypes consist incompatible genotypes
int jobo::get_n_unviable_species(
     std::vector<genotype> vector_of_genotypes
)
{
   genotype ab = vector_of_genotypes[1];
   const int sz{static_cast<int>(ab.size())};
   const int gsz{static_cast<int>(vector_of_genotypes.size())};
   int n_unviable_species{0};
   for (int i=0; i!=gsz; ++i)
   {
    const genotype z = vector_of_genotypes[i];
    for (int i=0; i!=sz; i+=2)
    {
      const char a{z[i+0]};
      const char b{z[i+1]};
      if (std::isupper(a) && std::isupper(b)) ++n_unviable_species;
    }
   }
   return n_unviable_species;
}

  // Function order
// 1. run_simulation
// 2. connect_generations
// 3. goto_next_generation
// 4. get_random_parents
// 5. create_offspring
// 6. recombine
// 7a.extinction_low_fitness
// 8. get_random_ints
// 9. create_mutation
// 10.mutation_check_all_loci
// 11.get_random_doubles
// 7b.extinction_low_fitness
// 13.calc_fitness
// 14.get_unique_genotypes
// 15.count_good_species
// 16.count_possible_species

  //Time
// Now time is counted in generations and all "steps" are the same
// # include time component to have differences in steps between the emergence
// of good and incipient species

  // Loci
// Maybe different mutation rate for each locus (not) necessary,
// Number of mutation rates dependent on loci
// Make it impossible for individual to have 1 individual as parents

  // Ideas / problems to think about
// 1. Possibility to choose parents in "species group of genotypes",
//    and not in the entire population
//    => choosing from the entire population prevents ignoring possible compatible genotypes
// 2. The recombination step could occur with blocks of loci and not per locus
//    => for each recombination step, first create random blocks of loci, same size?
// 3. An incompatible genotype doesn't always have to lead to death,
//    possibility for a threshold of incompatible loci couples,
//    before there is an effect on viability
//    => Or lower death chance or threshold or both?
// 4. The mutation step could occur for both parent before recombination,
//    and not in the child after recombination
//    => A mutation is more likely to occur in the reproduction process?

  //COUNT_INCIPIENT_SPECIES / GROUPS????
// I suggest a count_incipient_groups function to count the incipient groups:
// each of these groups would be counted as good species in the count_good_species function,
// if one or more genotypes would be removed.
// To close the gap between the BDM and the PBD model we could look at the possibilty
// to look back at previous generation to see which of the individuals
// from an incipient group were in the past counted as a good species and which individuals
// in the incipient group could be called incipient according to the PBD-model.
