#include "jobo_simulation.h"
#include "jobo_parameters.h"
#include "jobo_individuals.h"
#include "jobo_individual.h"
#include <random>
#include <iostream>
#include <vector>

jobo::simulation::simulation(
  const parameters& parameters
) noexcept : m_individuals(parameters.get_population_size()),
    m_parameters{parameters},
    m_rng_engine{42} //42 is the seed
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












