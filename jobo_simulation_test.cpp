#include "jobo_simulation_test.h"
#include "jobo_individual_test.h"
#include <cassert>
#include <string>
#include <stdexcept>
#include <iostream>
#include <vector>
#include "jobo_individual.h"
#include "jobo_parameters.h"
#include "jobo_simulation.h"

using namespace jobo;

int jobo::simulation_test() noexcept
{
  int n_fails{0};

  // Random ints are in the supposed range
  {
    const int n_loci{42};
    std::mt19937 rng_engine(42);
    std::vector<int> n_loci_ints = (get_random_ints(rng_engine, n_loci));
    for (int i=0; i!=n_loci; i+=1)
    {
      if (n_loci_ints[i] < 0) ++n_fails;
      if (n_loci_ints[i] > 100) ++n_fails;
    }
  }

  // Random doubles are in the supposed range
  {
    const int n_loci{42};
    std::mt19937 rng_engine(42);
    std::vector<double> n_loci_doubles = (get_random_doubles(rng_engine, n_loci));
    for (int i=0; i!=n_loci; i+=1)
    {
      if (n_loci_doubles[i] < 0) ++n_fails;
      if (n_loci_doubles[i] > 1) ++n_fails;
    }
  }

  //Test get_random_parent function
  {
    std::mt19937 rng_engine(42);
    const int population_size(10);
    const int number_of_parents = (population_size*2)-1;
    std::vector<int> random_parents = get_random_parents(rng_engine, population_size);
    for (int i=0; i!=number_of_parents; i+=1)
    {
      if (random_parents[i] < 0) ++n_fails;
      if (random_parents[i] > population_size) ++n_fails;
    }
  }

  // Test goto_next_generation function
  {
    const double mutation_rate (0.5);
    std::mt19937 rng_engine(42);
    std::vector<individual> individuals(20, individual("abcdefgh"));
    std::vector<individual> new_individuals = goto_next_generation(
    individuals,mutation_rate,rng_engine);
    {
      if (individuals.size() != new_individuals.size()) ++n_fails;
    }
  }

  // Test if individuals differ from new_individuals
  // around 75& for mutation_rate=0.5 at 2 loci
  {
    const double mutation_rate (0.5);
    std::mt19937 rng_engine(42);
    std::vector<individual> individuals(100, individual("ab"));
    const int population_size{static_cast<int>(individuals.size())};
    std::vector<individual> new_individuals = goto_next_generation(
    individuals,mutation_rate,rng_engine);
    {
      if (individuals.size() != new_individuals.size()) ++n_fails;
      int n_mutations{0};
      for (int i=0; i!= population_size; ++i)
      {
        if (individuals[i] != new_individuals[i]) ++n_mutations;
      }
      if (n_mutations <= 65) ++n_fails;
      if (n_mutations >= 85) ++n_fails;
    }
  }

  // Test extinction_low_fitnes
  {
    const double mutation_rate (0.5);
    std::mt19937 rng_engine(42);
    std::vector<individual> individuals(100, individual("ab"));
    std::vector<individual> new_individuals = goto_next_generation(
    individuals,mutation_rate,rng_engine);
    std::vector<individual> living_individuals = extinction_low_fitness(new_individuals);
    if (new_individuals.size() == living_individuals.size()) ++n_fails;
  }

  // Test connect_generations
  {
    const double mutation_rate (0.5);
    const int generations (1);
    std::mt19937 rng_engine(42);
    std::vector<individual> individuals(100, individual("ab"));
    std::vector<individual> new_individuals = goto_next_generation(
    individuals,mutation_rate,rng_engine);
    std::vector<individual> living_individuals = extinction_low_fitness(new_individuals);
    individuals = connect_generations(individuals, mutation_rate,rng_engine,generations);
    if (individuals.size() == living_individuals.size()) ++n_fails;
    if (generations < 1) ++n_fails;
  }

  return n_fails;
}










