#include "jobo_simulation.h"
#include "jobo_parameters.h"
#include "jobo_individuals.h"
#include "jobo_individual.h"
#include <cassert>
#include <iostream>
#include <vector>

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

/*
std::vector <individual> jobo::goto_next_generation(
  individuals,
  genotype,
  const int population_size,
  const double mutation_rate,
  std::mt19937& rng_engine
)
{
  assert(!m_individuals.empty());

  //Use create_offspring
  //Repeat create_offspring by the number of (old) population size

  //Get random father, pick random individual from vector
  const auto father = m_individuals.back(); //TODO
  //Get random mother, pick random individual from vector (not the same one)
  const auto mother = m_individuals.front(); //TODO
  //Create kid
  const auto kid = create_offspring(mother, father, m_rng_engine);


  //Replace all individuals with new offspring
  //Vector of individuals need to be only the "new kids"
  m_individuals.back() = kid; //TODO

  //Use create_mutation for genotype of each individual

  //Use calc_fitness to get fitness level of each genotype
  //Translate fitness to extinction for incompatible genotypes

  //Create extinction possibility for incipient & good species

  //Build in time/mutation step (other function)
}
*/

