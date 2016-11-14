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
#include <boost/test/unit_test.hpp>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

using namespace jobo;

BOOST_AUTO_TEST_CASE(test_jobo_setting_and_getting_parameters_should_be_symmetrical)
{
    //Setting and getting parameters should be symmetrical
    const parameters p(42,123,38,0.5,1);
    const simulation s(p);
    BOOST_CHECK(s.get_parameters()==p);
}

BOOST_AUTO_TEST_CASE(test_jobo_starting_simulation_should_have_right_population_size)
{
    //A starting simulation should have the right population size
    const parameters p(42,123,38,0.5,1);
    const simulation s(p);
    BOOST_CHECK(static_cast<int>(s.get_individuals().size())==p.get_population_size());
}

BOOST_AUTO_TEST_CASE(test_jobo_starting_population_has_only_individuals_of_the_same_genotype)
{
    //A starting population has individuals all of the same genotype
    const parameters p(42,123,38,0.5,1);
    const simulation s(p);
    const auto population = s.get_individuals();
    BOOST_CHECK(population.front() == population.back());
}

BOOST_AUTO_TEST_CASE(test_jobo_random_ints_are_in_the_supposed_range)
{
    //Random ints are in the supposed range
    const int n_loci{42};
    std::mt19937 rng_engine(42);
    std::vector<int> n_loci_ints = (get_random_ints(rng_engine, n_loci));
    for (int i=0; i!=n_loci; i+=1)
    {
      BOOST_CHECK(n_loci_ints[i] >= 0);
      BOOST_CHECK(n_loci_ints[i] <= 100);
    }
}

BOOST_AUTO_TEST_CASE(test_jobo_random_doubles_are_in_the_supposed_range)
{
    //Random doubles are in the supposed range
    const int n_loci{42};
    std::mt19937 rng_engine(42);
    std::vector<double> n_loci_doubles = (get_random_doubles(rng_engine, n_loci));
    for (int i=0; i!=n_loci; i+=1)
    {
      BOOST_CHECK(n_loci_doubles[i] > 0);
      BOOST_CHECK(n_loci_doubles[i] < 1);
    }
}

BOOST_AUTO_TEST_CASE(test_jobo_get_random_parent_function)
{
    //Test get_random_parent function
    std::mt19937 rng_engine(42);
    const int population_size(10);
    const int number_of_parents = (population_size*2)-1;
    std::vector<int> random_parents = get_random_parents(rng_engine, population_size);
    for (int i=0; i!=number_of_parents; i+=1)
    {
      BOOST_CHECK(random_parents[i] >= 0);
      BOOST_CHECK(random_parents[i] <= population_size);
    }
}

BOOST_AUTO_TEST_CASE(test_jobo_goto_next_generation_function)
{
    // Test goto_next_generation function
    const double mutation_rate (0.5);
    std::mt19937 rng_engine(42);
    std::vector<individual> individuals(20, individual("abcdefgh"));
    const int n_individuals{static_cast<int>(individuals.size())};
    BOOST_CHECK(n_individuals > 1);
    std::vector<individual> new_individuals = goto_next_generation(
    individuals,mutation_rate,rng_engine);
    BOOST_CHECK(individuals.size()!=new_individuals.size());
}


BOOST_AUTO_TEST_CASE(test_jobo_difference_individuals_and_new_individuals_around_75_procent)
{
    // Test if individuals differ from new_individuals
    // around 75% for mutation_rate=0.5 at 2 loci
    const double mutation_rate (0.5);
    std::mt19937 rng_engine(42);
    std::vector<individual> individuals(100, individual("ab"));
    const int population_size{static_cast<int>(individuals.size())};
    std::vector<individual> new_individuals = goto_next_generation(
    individuals,mutation_rate,rng_engine);
    BOOST_CHECK(individuals.size() == new_individuals.size());
    int n_mutations{0};
    for (int i=0; i!= population_size; ++i)
    {
      if (individuals[i] != new_individuals[i]) ++n_mutations;
    }
    BOOST_CHECK(n_mutations >= 65);
    BOOST_CHECK(n_mutations <= 85);
}

BOOST_AUTO_TEST_CASE(test_jobo_extinction_low_fitnes)
{
    // Test extinction_low_fitnes
    const double mutation_rate (0.5);
    std::mt19937 rng_engine(42);
    std::vector<individual> individuals(5, individual("abcd"));
    std::vector<individual> new_individuals = goto_next_generation(
    individuals,mutation_rate,rng_engine);
    std::vector<individual> living_individuals = extinction_low_fitness(new_individuals);
    BOOST_CHECK(new_individuals.size() != living_individuals.size());
}

BOOST_AUTO_TEST_CASE(test_jobo_connect_generations)
{
    // Test connect_generations
    const double mutation_rate (0.5);
    const int generations (1);
    std::mt19937 rng_engine(42);
    std::vector<individual> individuals(5, individual("abcd"));
    std::vector<individual> new_individuals = goto_next_generation(
    individuals,mutation_rate,rng_engine);
    std::vector<individual> living_individuals = extinction_low_fitness(new_individuals);
    individuals = connect_generations(individuals, mutation_rate,rng_engine);
    BOOST_CHECK(individuals.size() != living_individuals.size());
    BOOST_CHECK(generations >= 1);
}

BOOST_AUTO_TEST_CASE(test_jobo_calc_chance_dead_kids)
{
    //Test calc_chance_dead_kids
    const genotype w("abCd");
    const genotype q("AbCd");
    const genotype s("aBcD");
    double chance_dead_kids ;
    chance_dead_kids = calc_chance_dead_kids(w,q);
    BOOST_CHECK(chance_dead_kids == 0);
    chance_dead_kids = calc_chance_dead_kids(w,s);
    BOOST_CHECK(chance_dead_kids ==(0.50/2));
}

BOOST_AUTO_TEST_CASE(test_jobo_get_n_species)
{
    //Test get_n_species
    for (int i=0; i!=100; ++i)
    {
      std::set<genotype> set_of_genotypes = create_test_population_1(i);
      int n_genotypes{static_cast<int>(set_of_genotypes.size())};
      BOOST_CHECK(n_genotypes >=1);
    }
}

BOOST_AUTO_TEST_CASE(test_jobo_get_n_good_species)
{
    //Test get_n_good_species

    {
      std::set<genotype> set_of_genotypes{"AbCd","aBcD"};
      std::vector<double> chances_dead_kids = get_chances_dead_kids(set_of_genotypes);
      int n_good_species = get_n_good_species(chances_dead_kids,set_of_genotypes);
      BOOST_CHECK_EQUAL (n_good_species,2);
    }

    {
      std::set<genotype> set_of_genotypes{"Abcd","AbCd","abCd","abcd","aBCd","aBcD"};
      std::vector<double> chances_dead_kids = get_chances_dead_kids(set_of_genotypes);
      int n_good_species = get_n_good_species(chances_dead_kids,set_of_genotypes);
      BOOST_CHECK_EQUAL (n_good_species,1);
    }
    {
      std::set<genotype> set_of_genotypes{"Abcd","AbCd","abCd","aBCd","aBcD"};
      std::vector<double> chances_dead_kids = get_chances_dead_kids(set_of_genotypes);
      int n_good_species = get_n_good_species(chances_dead_kids,set_of_genotypes);
      BOOST_CHECK_EQUAL (n_good_species,2);
    }
    {
      std::set<genotype> set_of_genotypes{"Abcd","AbCd","abCd","abcd","aBCd"};
      std::vector<double> chances_dead_kids = get_chances_dead_kids(set_of_genotypes);
      int n_good_species = get_n_good_species(chances_dead_kids,set_of_genotypes);
      BOOST_CHECK_EQUAL (n_good_species,1);
    }

    for (int i=0; i!=100; ++i)
    {
      std::set<genotype> set_of_genotypes = create_test_population_1(i);
      int n_genotypes{static_cast<int>(set_of_genotypes.size())};
      std::vector<double> chances_dead_kids = get_chances_dead_kids(set_of_genotypes);
      int n_good_species = get_n_good_species(chances_dead_kids,set_of_genotypes);
      BOOST_CHECK (n_good_species <= n_genotypes);
    }
}

BOOST_AUTO_TEST_CASE(test_jobo_get_n_incipient_species)
{
    //Test get_n_incipient_species
    for (int i=0; i!=100; ++i)
    {
      std::set<genotype> set_of_genotypes = create_test_population_1(i);
      int n_genotypes{static_cast<int>(set_of_genotypes.size())};
      std::vector<double> chances_dead_kids = get_chances_dead_kids(set_of_genotypes);
      int n_incipient_species = get_n_incipient_species(chances_dead_kids,set_of_genotypes);
      BOOST_CHECK (n_incipient_species <= n_genotypes);
    }

    {
      std::set<genotype> set_of_genotypes{"Abcd","AbCd","abCd","abcd","aBCd"};
      std::vector<double> chances_dead_kids = get_chances_dead_kids(set_of_genotypes);
      int n_incipient_species = get_n_incipient_species(chances_dead_kids,set_of_genotypes);
      BOOST_CHECK_EQUAL (n_incipient_species,0);
    }
    {
      std::set<genotype> set_of_genotypes{"Abcd","AbCd","abCd","abcd","aBCd","aBcD"};
      std::vector<double> chances_dead_kids = get_chances_dead_kids(set_of_genotypes);
      int n_incipient_species = get_n_incipient_species(chances_dead_kids,set_of_genotypes);
      BOOST_CHECK_EQUAL (n_incipient_species,1);
    }
    {
      std::set<genotype> set_of_genotypes{"Abcd","AbCd","abCd","aBCd","aBcD"};
      std::vector<double> chances_dead_kids = get_chances_dead_kids(set_of_genotypes);
      int n_incipient_species = get_n_incipient_species(chances_dead_kids,set_of_genotypes);
      BOOST_CHECK_EQUAL (n_incipient_species,1);
    }
    {
      std::set<genotype> set_of_genotypes{"Abcd","AbCd","abCd","aBCd","aBcD","AbcD"};
      std::vector<double> chances_dead_kids = get_chances_dead_kids(set_of_genotypes);
      int n_incipient_species = get_n_incipient_species(chances_dead_kids,set_of_genotypes);
      BOOST_CHECK_EQUAL (n_incipient_species,2);
    }

}

BOOST_AUTO_TEST_CASE(test_jobo_different_genotypes_equal_to_incipient_genotypes_plus_good_genotypes)
{
    //Test different genotypes == incipient genotypes + good genotypes
    for (int i=0; i!=100; ++i)
    {
      std::set<genotype> set_of_genotypes = create_test_population_1(i);
      std::vector<double> chances_dead_kids = get_chances_dead_kids(set_of_genotypes);
      int n_good_species = get_n_good_species(chances_dead_kids,set_of_genotypes);
      int n_incipient_species = get_n_incipient_species(chances_dead_kids,set_of_genotypes);
      const int n_species{static_cast<int>(set_of_genotypes.size())};
      BOOST_CHECK (n_good_species + n_incipient_species == n_species);
    }
}

BOOST_AUTO_TEST_CASE(test_jobo_for_inviable_species_being_present)
{
    //Test for inviable species being present
    for (int i=0; i!=100; ++i)
    {
      std::set<genotype> set_of_genotypes = create_test_population_1(i);
      const int gsz{static_cast<int>(set_of_genotypes.size())};
      for (int i=0; i!=gsz; ++i)
      {
        BOOST_CHECK(get_n_unviable_species(set_of_genotypes) == 0);
      }
    }
}

BOOST_AUTO_TEST_CASE(test_jobo_for_generations_and_create_output_to_look_at_things)
{
    //Test for generations and create output
    const double mutation_rate (0.5);
    int generations (0);
    const int time (20);
    std::mt19937 rng_engine(42);
    std::vector<individual> individuals(100, individual("abcd"));
    std::set<genotype> set_of_genotypes;

    //Create output
    create_output_with_cout(time, mutation_rate, rng_engine, generations, individuals);

    for (int i=0; i!=time; ++i)
    {
      individuals = connect_generations(individuals,mutation_rate,rng_engine);
      generations = generations+1;
      BOOST_CHECK (generations >= 1);
      BOOST_CHECK (generations <= time);

      //Stop simulation if population size is 1
      if (individuals.size() == 1)
      {
        break;
      }

      //Count genotypes
      set_of_genotypes = get_n_species(individuals);
      BOOST_CHECK (set_of_genotypes.size() <= individuals.size());
    }
}

#pragma GCC diagnostic pop











