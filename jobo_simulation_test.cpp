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
    std::vector<individual> individuals(25, individual("abcd"));
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
    std::vector<individual> individuals(25, individual("abcd"));
    std::vector<individual> new_individuals = goto_next_generation(
    individuals,mutation_rate,rng_engine);
    std::vector<individual> living_individuals = extinction_low_fitness(new_individuals);
    individuals = connect_generations(individuals, mutation_rate,rng_engine);
    if (individuals.size() == living_individuals.size()) ++n_fails;
    if (generations < 1) ++n_fails;
  }

  //Test calc_chance_dead_kids
  {
  const genotype w("abCd");
  const genotype q("AbCd");
  const genotype s("aBcD");
  double chance_dead_kids ;
  chance_dead_kids = calc_chance_dead_kids(w,q);
  if(chance_dead_kids != 0) ++n_fails;
  chance_dead_kids = calc_chance_dead_kids(w,s);
  if(chance_dead_kids != (0.50/2)) ++n_fails;
  }

  //Test for multiple generations
  {
    const double mutation_rate (0.5);
    int generations (0);
    const int time (100);
    std::mt19937 rng_engine(42);
    std::vector<individual> individuals(100, individual("abcd"));

    std::set<genotype> set_of_genotypes;
    std::cout << "Generation: 0 "<< '\n';
    std::cout << "Number of individuals: " << individuals.size() << '\n';

    for (int i=0; i!=time; ++i)
    {
      individuals = connect_generations(individuals,mutation_rate,rng_engine);
      int n_individuals{static_cast<int>(individuals.size())};
      for (int i=0; i!=n_individuals; ++i)
      {
        const individual w = individuals[i];
        assert(w.get_genotype() != "ABCDEF");
      }
      generations = update_generations(generations);
      if (generations < 1) ++n_fails;
      if (generations > time) ++n_fails;

      //Show extinction process of the populations
      std::cout << "Generation: " << generations << '\n';
      std::cout << "Number of individuals: " << individuals.size() << '\n';

      //Stop simulation if population size is 1
      if (individuals.size() == 1)
      {
        break;
      }
      //Count genotypes
      set_of_genotypes = count_genotypes(individuals);
      if (set_of_genotypes.size() < 1) ++n_fails;
      if (set_of_genotypes.size() > individuals.size()) ++n_fails;
    }
  }
  return n_fails;
}










