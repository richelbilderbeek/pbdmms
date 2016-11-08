#include "jobo_simulation_test.h"
#include "jobo_individual_test.h"
#include <cassert>
#include <string>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <cctype>
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
    const int n_individuals{static_cast<int>(individuals.size())};
    assert (n_individuals > 1);
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
    std::vector<individual> individuals(5, individual("abcd"));
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
    std::vector<individual> individuals(5, individual("abcd"));
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

  //Test get_n_species
  {
    for (int i=0; i!=100; ++i)
    {
      std::set<genotype> set_of_genotypes = create_test_population_1(i);
      int n_genotypes{static_cast<int>(set_of_genotypes.size())};
      if (n_genotypes < 1) ++n_fails;
    }
  }

  //Test get_n_good_species
  {
    for (int i=0; i!=100; ++i)
    {
      std::set<genotype> set_of_genotypes = create_test_population_1(i);
      int n_genotypes{static_cast<int>(set_of_genotypes.size())};
      int n_good_species = get_n_good_species(set_of_genotypes);
      if (n_good_species >= n_genotypes) ++n_fails;
    }
  }

  //Test get_n_incipient_species

  {
    for (int i=0; i!=100; ++i)
    {
      std::set<genotype> set_of_genotypes = create_test_population_1(i);
      int n_genotypes{static_cast<int>(set_of_genotypes.size())};
      int n_good_species = get_n_good_species(set_of_genotypes);
      int n_incipient_species = get_n_incipient_species(n_good_species,set_of_genotypes);
      if (n_incipient_species >= n_genotypes) ++n_fails;
      if (n_incipient_species + n_good_species != n_genotypes) ++n_fails;
    }
  }


  //Test if inviable species are present in population
  {
    for (int i=0; i!=10; ++i)
    {
      std::set<genotype> set_of_genotypes = create_test_population_1(i);
      std::vector<std::string> vector_of_genotypes(set_of_genotypes.begin(), set_of_genotypes.end());
      const int gsz{static_cast<int>(set_of_genotypes.size())};
      //genotype a = vector_of_genotypes[1];
      //const int sz{static_cast<int>(a.size())};
      for (int i=0; i!=gsz; ++i)
      {
        std::string z = vector_of_genotypes[i];
        const int sz{static_cast<int>(z.size())};
        for (int i=0; i!=sz; i+=2)
        {
          const char a{z[i+0]};
          const char b{z[i+1]};
          if (std::isupper(a) && std::isupper(b)) ++n_fails;
        }
      }
    }
  }


  //Test for different genotypes == incipient genotypes + good genotypes
  {
    for (int i=0; i!=100; ++i)
    {
      std::set<genotype> set_of_genotypes = create_test_population_1(i);
      int n_good_species = get_n_good_species(set_of_genotypes);
      int n_incipient_species = get_n_incipient_species(n_good_species,set_of_genotypes);
      const int n_species{static_cast<int>(set_of_genotypes.size())};
      assert(n_good_species + n_incipient_species == n_species);
    }
  }


  //Test for inviable species being present
  {
    for (int i=0; i!=100; ++i)
    {
      std::set<genotype> set_of_genotypes = create_test_population_1(i);
      const int gsz{static_cast<int>(set_of_genotypes.size())};
      for (int i=0; i!=gsz; ++i)
      {
        assert(get_n_unviable_species(set_of_genotypes) == 0);
      }
    }
  }

  //Test for multiple generations
  {
    const double mutation_rate (0.5);
    int generations (0);
    const int time (100);
    std::mt19937 rng_engine(42);
    std::vector<individual> individuals(100, individual("abcdefgh"));

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
      set_of_genotypes = get_n_species(individuals);
      if (set_of_genotypes.size() < 1) ++n_fails;
      if (set_of_genotypes.size() > individuals.size()) ++n_fails;

      //Test get_n_good_species
      int n_species = static_cast<int>(set_of_genotypes.size());
      int n_good_species = get_n_good_species(set_of_genotypes);
      if(n_good_species > static_cast<int>(set_of_genotypes.size())) ++n_fails;

      std::cout << "Number of species: " << n_species << '\n';
      std::cout << "Number of 'good' species: " << n_good_species << '\n';
       std::cout << "Number of 'incipient' species: " << (set_of_genotypes.size()-n_good_species) << '\n' <<  '\n';
    }
  }
  return n_fails;
}










