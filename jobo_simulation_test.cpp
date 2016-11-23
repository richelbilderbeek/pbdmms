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

BOOST_AUTO_TEST_CASE(test_jobo_vectorting_and_getting_parameters_should_be_symmetrical)
{
    //Vectorting and getting parameters should be symmetrical
    const vector<individual> individuals{10, individual("aBCdEfG")};
    const parameters p(42,123,38,0.5,10, individuals);
    const simulation s(p);
    BOOST_CHECK(s.get_parameters()==p);
}

BOOST_AUTO_TEST_CASE(test_jobo_starting_simulation_should_have_right_population_size)
{
    //A starting simulation should have the right population size
    const vector<individual> individuals{10, individual("aBCdEfG")};
    const parameters p(42,123,38,0.5,10,individuals);
    const simulation s(p);
    BOOST_CHECK(static_cast<int>(s.get_individuals().size())==p.get_population_size());
}

BOOST_AUTO_TEST_CASE(test_jobo_starting_population_has_only_individuals_of_the_same_genotype)
{
    //A starting population has individuals all of the same genotype
    const vector<individual> individuals{10, individual("aBCdEfG")};
    const parameters p(42,123,38,0.5,10, individuals);
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

BOOST_AUTO_TEST_CASE(test_jobo_random_ints_with_negative_number_of_loci)
{
    std::mt19937 rng_engine(42);
    const int n_loci{-123};
    BOOST_CHECK_THROW(
    get_random_ints(rng_engine, n_loci),
    std::invalid_argument
    );
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

BOOST_AUTO_TEST_CASE(test_jobo_random_doubles_with_negative_number_of_loci)
{
    std::mt19937 rng_engine(42);
    const int n_loci{-123};
    BOOST_CHECK_THROW(
    get_random_doubles(rng_engine, n_loci),
    std::invalid_argument
    );
}

BOOST_AUTO_TEST_CASE(test_jobo_get_random_parent_function)
{
    //Test get_random_parent function
    std::mt19937 rng_engine(42);
    const int population_size(10);
    const int number_of_parents = (population_size*2)-1;
    std::vector<int> random_parents = get_random_parents(rng_engine, population_size);
    for (int i=0; i!=number_of_parents; ++i)
    {
      BOOST_CHECK(random_parents[i] >= 0);
      BOOST_CHECK(random_parents[i] < population_size);
    }
}

BOOST_AUTO_TEST_CASE(test_jobo_get_random_parent_abuse)
{
    //Cannot draw two different parents from a population of size one
    {
        std::mt19937 rng_engine(42);
        const int population_size{1}; //Cannot draw different father and mother
        BOOST_CHECK_THROW(
          get_random_parents(rng_engine, population_size),
          std::invalid_argument
        );
    }
    //Cannot draw parents from an empty population
    {
        std::mt19937 rng_engine(42);
        const int population_size{0}; //Cannot draw a father nor mother
        BOOST_CHECK_THROW(
          get_random_parents(rng_engine, population_size),
          std::invalid_argument
        );
    }
}

BOOST_AUTO_TEST_CASE(test_jobo_goto_next_generation_function)
{
    // Test goto_next_generation function
    const double mutation_rate{0.5};
    std::mt19937 rng_engine(42);
    const std::vector<individual> old_individuals(20, individual("abcdefgh"));
    const int n_individuals{static_cast<int>(old_individuals.size())};
    BOOST_CHECK(n_individuals > 1);
    const std::vector<individual> new_individuals = goto_next_generation(
      old_individuals,
      mutation_rate,
      rng_engine
    );
    BOOST_CHECK(old_individuals.size() != new_individuals.size());
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
    // Test calc_chance_dead_kids
    const genotype w("abCd");
    const genotype q("AbCd");
    const genotype s("aBcD");
    double chance_dead_kids ;
    chance_dead_kids = calc_chance_dead_kids(w,q);
    BOOST_CHECK(chance_dead_kids == 0);
    chance_dead_kids = calc_chance_dead_kids(q,s);
    BOOST_CHECK(chance_dead_kids ==(0.50/2));
}

BOOST_AUTO_TEST_CASE(test_jobo_get_unique_genotypes)
{
    vector<individual> individuals(10, individual("abcd"));
    vector<genotype> vector_of_genotypes = get_unique_genotypes(individuals);
    const int n_genotypes{static_cast<int>(vector_of_genotypes.size())};
    BOOST_CHECK(n_genotypes > 0);
}

BOOST_AUTO_TEST_CASE(test_jobo_count_good_species)
{
    // Test count_good_species
    {
      std::vector<individual> individuals;
      individual a{"AbCd"};
      individual b{"aBcD"};
      individuals.push_back(a);
      individuals.push_back(b);
      assert (individuals.size() != 0);
      int n_good_species = count_good_species(individuals);
      BOOST_CHECK_EQUAL (n_good_species,2);
    }
    {
      std::vector<individual> individuals;
      individual a{"Abcd"};
      individual b{"AbCd"};
      individual c{"abCd"};
      individual d{"abcd"};
      individual e{"aBCd"};
      individual f{"aBcD"};
      individuals.push_back(a);
      individuals.push_back(b);
      individuals.push_back(c);
      individuals.push_back(d);
      individuals.push_back(e);
      individuals.push_back(f);
      assert (individuals.size() != 0);
      assert (individuals.size() == 6);
      assert (individuals.size() != 8);
      int n_good_species = count_good_species(individuals);
      BOOST_CHECK_EQUAL (n_good_species,1);
    }
    {
      std::vector<individual> individuals;
      individual a{"Abcd"};
      individual b{"AbCd"};
      individual c{"abCd"};
      individual d{"aBCd"};
      individual e{"aBcD"};
      individuals.push_back(a);
      individuals.push_back(b);
      individuals.push_back(c);
      individuals.push_back(d);
      individuals.push_back(e);
      assert (individuals.size() != 0);
      assert (individuals.size() == 5);
      int n_good_species = count_good_species(individuals);
      BOOST_CHECK_EQUAL (n_good_species,2);
    }
    {
      std::vector<individual> individuals;
      individual a{"Abcd"};
      individual b{"AbCd"};
      individual c{"abCd"};
      individual d{"abcd"};
      individual e{"aBCd"};
      individuals.push_back(a);
      individuals.push_back(b);
      individuals.push_back(c);
      individuals.push_back(d);
      individuals.push_back(e);
      assert (individuals.size() != 0);
      assert (individuals.size() == 5);
      int n_good_species = count_good_species(individuals);
      BOOST_CHECK_EQUAL (n_good_species,1);
    }

    for (int i=0; i!=100; ++i)
    {
      std::vector<individual> individuals;
      std::vector<genotype> vector_of_genotypes = create_test_population_1(i);
      int n_genotypes{static_cast<int>(vector_of_genotypes.size())};
      int n_good_species = count_good_species(individuals);
      BOOST_CHECK (n_good_species <= n_genotypes);
    }
}

BOOST_AUTO_TEST_CASE(test_jobo_count_possible_species)
{
    // Test count_possible_species
    for (int i=0; i!=100; ++i)
    {
      std::vector<individual> individuals;
      std::vector<genotype> vector_of_genotypes = create_test_population_1(i);
      int n_genotypes{static_cast<int>(vector_of_genotypes.size())};
      int n_possible_species = count_possible_species(individuals);
      BOOST_CHECK (n_possible_species <= n_genotypes);
    }

    {
      std::vector<individual> individuals;
      individual a{"Abcd"};
      individual b{"AbCd"};
      individual c{"abCd"};
      individual d{"abcd"};
      individual e{"aBCd"};
      individuals.push_back(a);
      individuals.push_back(b);
      individuals.push_back(c);
      individuals.push_back(d);
      individuals.push_back(e);
      int n_possible_species = count_possible_species(individuals);
      BOOST_CHECK_EQUAL (n_possible_species,2);
    }
    { std::vector<individual> individuals;
      individual a{"Abcd"};
      individual b{"abCd"};
      individual c{"aBCd"};
      individuals.push_back(a);
      individuals.push_back(b);
      individuals.push_back(c);
      int n_possible_species = count_possible_species(individuals);
      BOOST_CHECK_EQUAL (n_possible_species,2);
    }
    {
      std::vector<individual> individuals;
      individual a{"Abcd"};
      individual b{"AbCd"};
      individual c{"abCd"};
      individual d{"abcd"};
      individual e{"aBCd"};
      individual f{"aBcD"};
      individuals.push_back(a);
      individuals.push_back(b);
      individuals.push_back(c);
      individuals.push_back(d);
      individuals.push_back(e);
      individuals.push_back(f);
      int n_possible_species = count_possible_species(individuals);
      BOOST_CHECK_EQUAL (n_possible_species,3);
    }
    {
      std::vector<individual> individuals;
      individual a{"Abcd"};
      individual b{"AbCd"};
      individual c{"abCd"};
      individual d{"aBCd"};
      individual e{"aBcD"};
      individuals.push_back(a);
      individuals.push_back(b);
      individuals.push_back(c);
      individuals.push_back(d);
      individuals.push_back(e);
      int n_possible_species = count_possible_species(individuals);
      BOOST_CHECK_EQUAL (n_possible_species,3);
    }
    {
      std::vector<individual> individuals;
      individual a{"abcd"};
      individual b{"Abcd"};
      individual c{"AbCd"};
      individual d{"aBCd"};
      individual e{"aBcD"};
      individuals.push_back(a);
      individuals.push_back(b);
      individuals.push_back(c);
      individuals.push_back(d);
      individuals.push_back(e);
      int n_possible_species = count_possible_species(individuals);
      BOOST_CHECK_EQUAL (n_possible_species,3);
    }

    //#define FIX_ISSUE_58
    #ifdef FIX_ISSUE_58
    // The test breaking count_possible_species
    {
        const std::vector<individual> population =
        {
            individual("abcDefgHIj"),
            individual("AbcDeFGhIj"),
            individual("aBcDeFGhIj"),
            individual("abcdefGhiJ"),
            individual("abcdefGhiJ"),
            individual("aBcdefGhij"),
            individual("abcDefGhiJ"),
            individual("abCdefGhiJ"),
            individual("aBCdefghij"),
            individual("abCdeFGhij"),
            individual("AbCdEfghiJ"),
            individual("abCdefGhIj"),
            individual("abcdefGhij"),
            individual("aBcDEfghij"),
            individual("abCdEfgHIj"),
            individual("aBcdefgHij"),
            individual("aBcdefgHij"),
            individual("aBcdefghij"),
            individual("abCdefGhiJ"),
            individual("abCdeFGhiJ"),
            individual("abcdefgHIj"),
            individual("aBcdefghij"),
            individual("abCdefGhij"),
            individual("abCdEfGhIj"),
            individual("abCdefGhiJ"),
            individual("aBcDEfghij"),
            individual("aBcdefgHij"),
            individual("abCdefghij"),
            individual("abCdeFGhIj"),
            individual("Abcdefghij"),
            individual("abCdEfgHIj"),
            individual("aBcdeFghiJ"),
            individual("abCdEfghij"),
            individual("aBcdefghij"),
            individual("AbcdefgHij"),
            individual("aBCdefghiJ"),
            individual("abcDefGhiJ"),
            individual("abcdEfghiJ"),
            individual("aBcdefgHij"),
            individual("aBcDEfgHij"),
            individual("abcdefgHij")
        };
        int n_possible_species = count_possible_species(population);
        BOOST_CHECK (n_possible_species >= 0);
    }
    #endif // FIX_ISSUE_58
}

BOOST_AUTO_TEST_CASE(test_jobo_for_create_test_population_1)
{
    // Test create_test_population function 1
    std::vector<genotype> vector_of_genotypes = create_test_population_1(0);
    BOOST_CHECK(vector_of_genotypes.size() == 1);
}

BOOST_AUTO_TEST_CASE(test_jobo_for_create_test_population_1_2)
{
    // Test create_test_population function 2
    std::vector<genotype> vector_of_genotypes = create_test_population_1(2);
    BOOST_CHECK(vector_of_genotypes.size() == 24);
}

BOOST_AUTO_TEST_CASE(test_jobo_for_inviable_species_being_present)
{
    // Test for inviable species being present
    for (int i=0; i!=10; ++i)
    {
      std::vector<genotype> vector_of_genotypes = create_test_population_1(i);
      const int gsz{static_cast<int>(vector_of_genotypes.size())};
      for (int i=0; i!=gsz; ++i)
      {
        BOOST_CHECK(get_n_unviable_species(vector_of_genotypes) == 0);
      }
    }
}

#pragma GCC diagnostic pop











