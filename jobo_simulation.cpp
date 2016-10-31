#include "jobo_simulation.h"
#include "jobo_parameters.h"
#include "jobo_individuals.h"
#include "jobo_individual.h"
#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>

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
    //assert(i+population_size < random_parents.size());
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

int jobo::update_generations(int generations
)
{
int new_generations = generations+1;
generations = new_generations;
return generations;
}

std::vector<individual> jobo::extinction_low_fitness(
  std::vector<individual> new_individuals
)
{
  //Loop through every individual of new_individuals to check fitness level
  const int population_size{static_cast<int>(new_individuals.size())};
  std::vector<int> fitness_levels;
  std::vector<individual> living_individuals;
  living_individuals = new_individuals;
  for (int i=0; i!=population_size; ++i)
  {
    const individual& k = new_individuals[i];
    //Use calc_fitness to get fitness level of each genotype (genotype needed)
    int n_low_fitness = calc_fitness(k.get_genotype());
    //make vector of fitness levels for each (new)individual
    fitness_levels.push_back(n_low_fitness);
  }

  const int f{static_cast<int>(fitness_levels.size()-1)};
  //auto f = fitness_levels.size()-1;
  for (int i=f; i!=-1; --i)
  {
    if (fitness_levels[i] == 0)
    {
      living_individuals.erase(living_individuals.begin()+i);
      fitness_levels.erase(fitness_levels.begin()+i);
    }
  }
  //Use fitness vector to remove individual(s) from new_individuals
  return living_individuals;
}

std::vector<individual> jobo::connect_generations(
    std::vector<individual> individuals,
    const double mutation_rate,
    std::mt19937& rng_engine
)
{
  //Make circle complete with goto_next_generation
  std::vector<individual> new_individuals = goto_next_generation(
  individuals,mutation_rate,rng_engine);
  std::vector<individual> living_individuals = extinction_low_fitness(new_individuals);
  //Translate living_individuals into individuals
  individuals = living_individuals;
  new_individuals = living_individuals;
  return individuals;
}

    //compensate extinction incompatibles with birth??
    //For every new time step/generations: store number of good and incipient species


