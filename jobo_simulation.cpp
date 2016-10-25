#include "jobo_simulation.h"
#include "jobo_parameters.h"
#include "jobo_individuals.h"
#include "jobo_individual.h"
#include <cassert>
#include <iostream>
#include <vector>

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
  //Need to use number of loci to get number of random int with 1 seed
  std::vector<int> n_loci_ints;
  n_loci_ints.resize(n);

  std::uniform_int_distribution<int> distribution(0,100);
  for (int i=0; i!=n; ++i)
    {
    int w = distribution(rng_engine);
    n_loci_ints[i] =  w;
   }
 //Need to get all random int in one return value
 return n_loci_ints;
}

std::vector<double> jobo::get_random_doubles(std::mt19937& rng_engine, int n)
{
  //Need to use number of loci to get number of random doubles with 1 seed
  std::vector<double> n_loci_doubles;
  n_loci_doubles.resize(n);

  std::uniform_real_distribution<double> distribution(0,1);
  for (int i=0; i!=n; ++i)
    {
    double w = distribution(rng_engine);
    n_loci_doubles[i] =  w;
   }
 //Need to get all random doubles in one return value
 return n_loci_doubles;
}

std::vector<int> jobo::get_random_parent(std::mt19937& rng_engine, int population_size)
{
  std::vector<int> get_random_parents;
  const int number_of_parents = population_size*2;
  get_random_parents.resize(number_of_parents);
  std::uniform_int_distribution<int> distribution(1,population_size);
  for (int i=0; i!=number_of_parents; ++i)
    {
    int w = distribution(rng_engine);
    get_random_parents[i] =  w;
    }  
    //TODO prevent that parents are same individual
  return get_random_parents;
}

std::vector<individual> jobo::goto_next_generation(
  std::vector<individual> individuals,
  //genotype,
  const int population_size,
  //const double mutation_rate,
  std::mt19937& rng_engine
)
{
  //TODO test if individual class is not empty
  //assert (!individual.empty());
  //Repeat create_offspring by the number of (old) population size
  for (int i=0; i!=population_size; i+=1)
    {
    //Get random number to select random individual
    std::vector<int> get_random_parents = (get_random_parent(rng_engine, population_size));
    //Get random father, pick random individual from vector
    int number_father = get_random_parents[i];
    individual father = individuals[number_father];
    //Get random mother, pick random individual from vector
    int number_mother = get_random_parents[i+population_size];
    individual mother = individuals[number_mother];
    //Create kid
    //TODO Try to create new vector with function create_offspring
    new_individual offspring = create_offspring(mother, father, rng_engine);
    std::vector<new_individual> new_individuals;
    new_individuals.push_back(offspring);
    //jobo::new_individuals = std::vector<offspring>;
    }
  //TODO Try to replace content vector individuals with content vector new_individuals
  //So function goto_next_generation returns the vector individuals with new generation
  for (int i=0; i!=population_size; i+=1)
  {
    individuals[i] = new_individuals[i];
  }
return individuals;
}

  //Replace all individuals with new offspring
  //Remove "old" vector individuals population
  //and replace it with new_individuals population
  //Vector of individuals need to be only the "new kids"!

  //Use create_mutation for genotype of each individual (mutation_rate needed)

  //Use calc_fitness to get fitness level of each genotype (genotype needed)
  //Translate fitness to extinction for incompatible genotypes

  //Create extinction possibility for incipient & good species
  //(not yet existing extinction_rate needed)

  //Build in time/mutation step (other function)

