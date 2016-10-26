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

std::vector<int> jobo::get_random_parents(
  std::mt19937& rng_engine,
  int population_size
)
{
  std::vector<int> get_random_parents;
  const int number_of_parents = population_size*2;
  get_random_parents.resize(number_of_parents);
  std::uniform_int_distribution<int> distribution(0,population_size-1);
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
  const double mutation_rate,
  std::mt19937& rng_engine
)
{
  const int population_size{static_cast<int>(individuals.size())};
  //Get random numbers to select random individuals
  const std::vector<int> random_parents = get_random_parents(rng_engine, population_size);
  //All random parent indices should be valid
  for (const int i: random_parents)
  {
    assert(i >= 0);
    assert(i < static_cast<int>(individuals.size()));
  }

  const int n_couples{static_cast<int>(random_parents.size()) / 2};
  assert(n_couples == population_size);
  std::vector<individual> new_individuals;

  //Repeat create_offspring by the number of couples
  for (int i=0; i!=n_couples; ++i)
  {
    //Get random father, pick random individual from vector
    const int number_father = random_parents[i];
    assert(number_father < static_cast<int>(individuals.size()));
    const individual father = individuals[number_father];
    //Get random mother, pick random individual from vector
    assert(i+population_size < random_parents.size());
    const int number_mother = random_parents[i+population_size];
    assert(number_mother < static_cast<int>(individuals.size()));
    const individual mother = individuals[number_mother];
    //Create kid
    const individual offspring = create_offspring(mother, father, rng_engine);
    new_individuals.push_back(offspring);
  }

  //Loop through every individual of new_individuals to check for mutation(s)
  for (int i=0; i!=population_size; ++i)
  {
    //Use create_mutation for genotype of each individual
    new_individuals[i] = create_mutation(new_individuals[i],mutation_rate,rng_engine);
  }

  return new_individuals;
}

  //Extintion
    //by incompatibility genotype
    //Use calc_fitness to get fitness level of each genotype (genotype needed)
    //Translate fitness to extinction for incompatible genotypes
    //compensate extinction incompatibles with birth??

  //Build in time/mutation step (other function)
    //For every new time step individuals will be
    //the new_individuals from the previous generation



