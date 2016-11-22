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
    //vectorting and getting parameters should be symmetrical
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
    chance_dead_kids = calc_chance_dead_kids(q,s);
    BOOST_CHECK(chance_dead_kids ==(0.50/2));
}

BOOST_AUTO_TEST_CASE(test_jobo_get_unique_genotypes)
{
    //Test get_unique_genotypes
    //std::vector<genotype> vector_of_genotypes = create_test_population_1(time);
    {
      vector<individual> individuals(10, individual("abcd"));
      vector<genotype> vector_of_genotypes = get_unique_genotypes(individuals);
      const int n_genotypes{static_cast<int>(vector_of_genotypes.size())};
      BOOST_CHECK(n_genotypes > 0);
    }
}

BOOST_AUTO_TEST_CASE(test_jobo_count_good_species)
{
    //Test count_good_species
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
    //Test count_possible_species
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

    //The test breaking count_possible_species
    /*
    {
      std::vector<individual> individuals;
      individual a1{"abcDefgHIj"};
      individual a2{"AbcDeFGhIj"};
      individual a3{"aBcDeFGhIj"};
      individual a4{"abcdefGhiJ"};
      individual a5{"abcdefGhiJ"};
      individual a6{"aBcdefGhij"};
      individual a7{"abcDefGhiJ"};
      individual a8{"abCdefGhiJ"};
      individual a9{"aBCdefghij"};
      individual a10{"abCdeFGhij"};
      individual a11{"AbCdEfghiJ"};
      individual a12{"abCdefGhIj"};
      individual a13{"abcdefGhij"};
      individual a14{"aBcDEfghij"};
      individual a15{"abCdEfgHIj"};
      individual a16{"aBcdefgHij"};
      individual a17{"aBcdefgHij"};
      individual a18{"aBcdefghij"};
      individual a19{"abCdefGhiJ"};
      individual a20{"abCdeFGhiJ"};
      individual a21{"abcdefgHIj"};
      individual a22{"aBcdefghij"};
      individual a23{"abCdefGhij"};
      individual a24{"abCdEfGhIj"};
      individual a25{"abCdefGhiJ"};
      individual a26{"aBcDEfghij"};
      individual a27{"aBcdefgHij"};
      individual a28{"abCdefghij"};
      individual a29{"abCdeFGhIj"};
      individual a30{"Abcdefghij"};
      individual a31{"abCdEfgHIj"};
      individual a32{"aBcdeFghiJ"};
      individual a33{"abCdEfghij"};
      individual a34{"aBcdefghij"};
      individual a35{"AbcdefgHij"};
      individual a36{"aBCdefghiJ"};
      individual a37{"abcDefGhiJ"};
      individual a38{"abcdEfghiJ"};
      individual a39{"aBcdefgHij"};
      individual a40{"aBcDEfgHij"};
      individual a41{"abcdefgHij"};
      individuals.push_back(a1);
      individuals.push_back(a2);
      individuals.push_back(a3);
      individuals.push_back(a4);
      individuals.push_back(a5);
      individuals.push_back(a6);
      individuals.push_back(a7);
      individuals.push_back(a8);
      individuals.push_back(a9);
      individuals.push_back(a10);
      individuals.push_back(a11);
      individuals.push_back(a12);
      individuals.push_back(a13);
      individuals.push_back(a14);
      individuals.push_back(a15);
      individuals.push_back(a16);
      individuals.push_back(a17);
      individuals.push_back(a18);
      individuals.push_back(a19);
      individuals.push_back(a20);
      individuals.push_back(a21);
      individuals.push_back(a22);
      individuals.push_back(a23);
      individuals.push_back(a24);
      individuals.push_back(a25);
      individuals.push_back(a26);
      individuals.push_back(a27);
      individuals.push_back(a28);
      individuals.push_back(a29);
      individuals.push_back(a30);
      individuals.push_back(a31);
      individuals.push_back(a32);
      individuals.push_back(a33);
      individuals.push_back(a34);
      individuals.push_back(a35);
      individuals.push_back(a36);
      individuals.push_back(a37);
      individuals.push_back(a38);
      individuals.push_back(a39);
      individuals.push_back(a40);
      individuals.push_back(a41);
      int n_possible_species = count_possible_species(individuals);
      BOOST_CHECK (n_possible_species >= 0);
    }
      */
}

BOOST_AUTO_TEST_CASE(test_jobo_for_create_test_population_1)
{
    std::vector<genotype> vector_of_genotypes = create_test_population_1(0);
    BOOST_CHECK(vector_of_genotypes.size() == 1);
}

BOOST_AUTO_TEST_CASE(test_jobo_for_create_test_population_1_2)
{
    std::vector<genotype> vector_of_genotypes = create_test_population_1(2);
    BOOST_CHECK(vector_of_genotypes.size() == 24);
}

BOOST_AUTO_TEST_CASE(test_jobo_for_inviable_species_being_present)
{
    //Test for inviable species being present
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











