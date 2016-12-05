#include "jobo_simulation.h"
#include "jobo_parameters.h"
#include "jobo_individuals.h"
#include "jobo_individual.h"
#include "jobo_results.h"
#include "jobo_jkr_adapters.h"
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
  : m_individuals{create_initial_population(parameters)},
    m_parameters{parameters},
    m_rng_engine(parameters.get_seed()),
    m_results{}
{
}

individuals jobo::create_initial_population(const parameters& parameters)
{
  const std::vector<individual> population(
    parameters.get_population_size(),
    individual(create_initial_genotype(parameters.get_n_loci()))
  );

  //Postconditions
  assert(static_cast<int>(population.size()) == parameters.get_population_size());
  assert(population.back().get_n_loci() == parameters.get_n_loci());
  return population;
}

void jobo::simulation::set_individuals(const individuals& is)
{
  this->m_individuals = is;
}

std::vector<int> jobo::get_random_ints(std::mt19937& rng_engine, int n)
{
  // Use number of loci to get number of random ints with 1 seed
  if (n < 0)
  {
    throw std::invalid_argument("number of ints must be positive");
  }
  std::vector<int> n_loci_ints;
  n_loci_ints.resize(n);
  std::uniform_int_distribution<int> distribution(0,100);
  for (int i=0; i!=n; ++i)
  {
    int w = distribution(rng_engine);
    assert (w >=0);
    assert (w <=100);
    n_loci_ints[i] =  w;
  }
  // Return all random ints in one vector
  return n_loci_ints;
}

std::vector<double> jobo::get_random_doubles(std::mt19937& rng_engine, int n)
{
  // Use number of loci to get number of random doubles with 1 seed
  if (n < 0)
  {
    throw std::invalid_argument("number of doubles must be positive");
  }
  std::vector<double> n_loci_doubles;
  n_loci_doubles.resize(n);
  std::uniform_real_distribution<double> distribution(0,1);
  for (int i=0; i!=n; ++i)
  {
    double w = distribution(rng_engine);
    assert (w >=0);
    assert (w <=1);
    n_loci_doubles[i] =  w;
  }
  // Return all random doubles in one vector
  return n_loci_doubles;
}

int jobo::get_random_parent(
    std::mt19937& rng_engine,
    int population_size
)
{
  std::uniform_int_distribution<int> distribution(0,population_size-1);
  int random_parent = distribution(rng_engine);
  return random_parent;
}

int jobo::count_capitals (std::string genotype)
{
  int capitals_in_genotype{0};
  const int genotype_size{static_cast<int>(genotype.size())};
  for (int i = 0; i < genotype_size; i++)
  {
    if ( genotype[i] >= 'A' && genotype[i] <= 'Z' )
      {
        capitals_in_genotype++;
      }
  }
  return capitals_in_genotype;
}

double jobo::calc_competition(
    std::vector<individual> individuals,
    const int i
    )
{
  double comp{0.0};
  const int sz{static_cast<int>(individuals.size())};
  for (int j=i+1; j!=sz; ++j)
  {
    assert(i >= 0);
    assert(i < sz);
    individual a = individuals[i];
    assert(j > 0);
    assert(j < sz);
    individual b = individuals[j];
    int n_genotype_i = std::count( individuals.begin(), individuals.end(), a.get_genotype());
    int n_genotype_j = std::count( individuals.begin(), individuals.end(), b.get_genotype());
    //number of genotypes i, number of genotypes j, population size
    comp+=gauss(n_genotype_i-n_genotype_j,sz);
  }
  return comp;
}

double jobo::calc_survivability(
    const double fitness_gen,
    const double comp,
    const int population_size
    )
{
  return (1.0 - (comp / population_size) / fitness_gen);
}

double jobo::gauss(int capitals_in_genotype, int max_capitals)
{ return exp(-(capitals_in_genotype*capitals_in_genotype)/(2.0*max_capitals*max_capitals));}

std::vector<individual> jobo::goto_next_generation(
  std::vector<individual> individuals,
  const double mutation_rate,
  std::mt19937& rng_engine
)
{
  const int population_size{static_cast<int>(individuals.size())};

  //TODO Get random numbers to select random individuals

  std::vector<individual> new_individuals;
  std::cout << "Starting loop.\n";
  // Repeat create_offspring by the number of constant population size
  while (static_cast<int>(new_individuals.size()) <= 100)
  {
    // Get random father, pick random individual from vector
    //vector<int> number_parents = get_random_parents(rng_engine,population_size);

    int number_father = get_random_parent(rng_engine,population_size);
    int number_mother;
    do {number_mother = get_random_parent(rng_engine,population_size);}
    // This do-while loop probably causes an endless loop
    while (number_father == number_mother);

    //int number_mother = number_parents[0];
    //int number_father = number_parents[1];
    assert(number_mother >= 0);
    assert(number_mother <= population_size);
    assert(number_father >= 0);
    assert(number_father <= population_size);
    //number_mother = get_random_parent(rng_engine,population_size);
    // TODO Parents can't be one and the same!
    assert(number_father != number_mother);
    const individual father = individuals[number_father];

    // Get random mother, pick random individual from vector
    const individual mother = individuals[number_mother];

    // Implement genetic impact on fitness
    // Count number of capitals in each genotype:
    int mother_capitals = count_capitals(mother.get_genotype());
    int father_capitals = count_capitals(father.get_genotype());
    string mother_genotype (mother.get_genotype());
    int max_capitals = static_cast<int>(mother_genotype.size()/2);

    // The more capitals, the lower the fitness
    // Translate effect capitals_in_genotype to effect on fitness
    double fitness_mother_gen (gauss(mother_capitals,max_capitals));
    double fitness_father_gen (gauss(father_capitals,max_capitals));
    assert (fitness_mother_gen <= 1);
    assert (fitness_mother_gen >= 0);
    assert (fitness_father_gen <= 1);
    assert (fitness_father_gen >= 0);

    // Implement population impact on fitness
    // Count number of individuals per genotype:
    // The more individuals of a genotype, the lower the fitness
    double fitness_mother_pop = calc_competition(individuals, number_mother);
    double fitness_father_pop = calc_competition(individuals, number_father);

    const int sz{static_cast<int>(individuals.size())};
    double fitness_mother = calc_survivability(fitness_mother_gen,fitness_mother_pop,sz);
    double fitness_father = calc_survivability(fitness_father_gen,fitness_father_pop,sz);
    assert (fitness_mother <= 1);
    assert (fitness_father <= 1);

    // check before create_offspring the fitness for each of the parents:
    // if both parents fitness is high enough, offspring is possible

    double fitness_threshold = 0.05;
    if (fitness_mother > fitness_threshold && fitness_father > fitness_threshold)

    {
      // Create kid
      const individual offspring = create_offspring(mother, father, rng_engine);
      if (offspring.get_genotype().size() % 2 != 0)
      {
        throw std::invalid_argument("genotype length must be even");
      }
      new_individuals.push_back(offspring);
    }
  }

  // After the recombination step the incompatible individuals die
  new_individuals = extinction_low_fitness(new_individuals);

  // Loop through every individual of new_individuals to check for mutation(s)
  for (int i=0; i!=static_cast<int>(new_individuals.size()); ++i)
  {
    // Use create_mutation for genotype of each individual
    assert(i >= 0);
    assert(i < static_cast<int>(new_individuals.size()));
    new_individuals[i] = create_mutation(new_individuals[i],mutation_rate,rng_engine);
  }

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
  int n_generations
)
{
  const double mutation_rate (0.5);
  mt19937 rng_engine(42);
  vector<individual> individuals(100, individual("abcdef"));
  for (int i=0; i!=n_generations; ++i)
  {
     individuals = connect_generations(individuals,mutation_rate,rng_engine);
     assert(individuals.size() > 1);
  }
  vector<genotype> vector_of_genotypes = get_unique_genotypes(individuals);
  assert(vector_of_genotypes.size() > 0);
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

/* Only used in first template function
void jobo::simulation::do_timestep()
{
  //Measure current generation (may be the initial population)
  const int n_good_species = count_good_species(m_individuals);

  m_results.add_ltt(n_good_species);

  const individuals next_generation = connect_generations(
    m_individuals,
    m_parameters.get_mutation_rate(),
    m_rng_engine
  );
  m_individuals = next_generation;
}
*/

  // Competition

// first look at incompatible genotypes and kill them (already present)



// Check at the start of reproduction the fitness of each individual parent to give the competition
// factor a certain impact on the population (to get disruptive selection).
// Each individual has it's own competition parameter/trait with a value between 0 and 10
// (depends on range of competition field).
// First generation individuals all have a random initial competition factor.
// ^^Each generation there are one or more "best" maximum competition value(s)
// ^^this maximum competition value (MCV) changes with 0.2 each generation
// ^An individual gets the competition parameter from one of it's parents,
// ^otherwise the competition parameter would go faster towards an average
// ^^^Examples: If an individual has a competition factor of 0.5 and the maximum competition value
//              is 0 and/or 1 the individuals chance to reproduce could decrease to 0.5
//              Or if the individuals competition factor is not in a range of 0.5 of the MCV,
//              the individuals change to reproduce will decrease
// How to implement competition:
// 1.Make for each individual a competition parameter/trait
// (just like the fitness trait of 0 or 1 in the current model)
// 2.Create two maximum competition values and make sure each MCS value changes each generation
// with steps of 0.2, so you have a changing/shifting competition optimum in your model
// 3.Before reproduction, check for each parent how far their competition parameter stands
// from the MCS and implement the consequences on their chance to reproduce.

  // Time
// Now time is counted in generations and all "steps" are the same
// include time component to have differences in steps between the emergence
// of good and incipient species

  // Loci
// Maybe different mutation rate for each locus (not) necessary,
// Number of mutation rates dependent on loci
// # Make it impossible for individual to have 1 individual as parents

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

  // Count_incipient_species / incipient_groups
// I suggest a count_incipient_groups function to count the incipient groups:
// each of these groups would be counted as good species in the count_good_species function,
// if one or more genotypes would be removed.
// To close the gap between the BDM and the PBD model we could look at the possibilty
// to look back at previous generation to see which of the individuals
// from an incipient group were in the past counted as a good species and which individuals
// in the incipient group could be called incipient according to the PBD-model.
