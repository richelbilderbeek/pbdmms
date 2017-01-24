#include <cassert>
#include <string>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <cctype>
#include "jobo_individual.h"
#include "jobo_parameters.h"
#include "jobo_simulation.h"
#include "jobo_genotypes.h"
#include "jobo_jkr_adapters.h"
#include "jobo_results.h"
#include <boost/test/unit_test.hpp>
#include <fstream>
#include "file_to_vector.h"
#include "is_regular_file.h"
#include "jkr_experiment.h"
#include "seperate_string.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

using namespace jobo;

//Short version, do not use
void delete_file_2(const std::string& filename)
{
  assert(is_regular_file(filename));
  std::remove(filename.c_str());
  assert(!is_regular_file(filename));
}

BOOST_AUTO_TEST_CASE(test_jobo_simulation_initial_population_should_have_the_right_size)
{
    const parameters p = create_test_parameters_1();
    const simulation s(p);
    const int n_individuals{static_cast<int>(s.get_individuals().size())};
    const int sz_population{static_cast<int>(p.get_population_size())};
    BOOST_CHECK(n_individuals == sz_population);
}

BOOST_AUTO_TEST_CASE(test_jobo_simulation_initial_population_has_genotype_of_the_right_size)
{
    const parameters p = create_test_parameters_1();
    const simulation s(p);
    assert(!s.get_individuals().empty());
    const individual i = s.get_individuals().back();
    const int sz_genotype{static_cast<int>(i.get_genotype().size())};
    BOOST_CHECK_EQUAL(sz_genotype, p.get_n_loci());
}

BOOST_AUTO_TEST_CASE(test_jobo_vectorting_and_getting_parameters_should_be_symmetrical)
{
    const parameters p = create_test_parameters_1();
    const simulation s(p);
    BOOST_CHECK_EQUAL(s.get_parameters(), p);
}

BOOST_AUTO_TEST_CASE(test_jobo_starting_simulation_should_have_right_population_size)
{
    const parameters p = create_test_parameters_1();
    const simulation s(p);
    BOOST_CHECK(static_cast<int>(s.get_individuals().size())==p.get_population_size());
}

BOOST_AUTO_TEST_CASE(test_jobo_starting_population_has_only_individuals_of_the_same_genotype)
{
    const parameters p = create_test_parameters_1();
    const simulation s(p);
    const auto population = s.get_individuals();

    BOOST_CHECK_EQUAL(
      std::count(std::begin(population), std::end(population), population.front()),
      p.get_population_size()
    );
}

BOOST_AUTO_TEST_CASE(test_jobo_random_ints_are_in_the_supposed_range)
{
    // Random ints are in the supposed range
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
    // Test get_random_ints for negative number of loci
    std::mt19937 rng_engine(42);
    const int n_loci{-123};
    BOOST_CHECK_THROW(
    get_random_ints(rng_engine, n_loci),
    std::invalid_argument
    );
}

BOOST_AUTO_TEST_CASE(test_jobo_random_doubles_are_in_the_supposed_range)
{
    // Random doubles are in the supposed range
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
    // Test get_random_doubles for negative number of loci
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
    int random_parent = get_random_parent(rng_engine, population_size);
    BOOST_CHECK(random_parent >= 0);
    BOOST_CHECK(random_parent < population_size);

}

BOOST_AUTO_TEST_CASE(test_jobo_goto_next_generation_function)
{
    const parameters ps = create_test_parameters_1();
    std::mt19937 rng_engine(ps.get_seed());
    const individuals current_population = create_initial_population(ps);
    const std::vector<individual> next_population = create_next_generation(
        current_population,
        ps,
        rng_engine
    );
    BOOST_CHECK_EQUAL(current_population.size(), next_population.size());
}


BOOST_AUTO_TEST_CASE(test_jobo_difference_individuals_and_new_individuals_around_75_procent)
{
    #ifdef FIX_ISSUE_140
    // Test if individuals differ from new_individuals
    // around 75% for mutation_rate=0.5 at 2 loci
    const parameters ps = create_test_parameters_1();
    std::mt19937 rng_engine(ps.get_seed());
    std::vector<individual> first_population = create_initial_population(ps);
    std::vector<individual> next_population = create_next_generation(
        first_population,
        ps,
        rng_engine
    );
    BOOST_CHECK_EQUAL(first_population.size(), next_population.size());
    const int expected_mutations{calc_expected_loci_mutations_per_generation(ps)};
    const int measured_mutations{measure_n_loci_mutations(first_population, second_population)};
    BOOST_CHECK(measured_mutations > expected_mutations / 2);
    BOOST_CHECK(measured_mutations < 3 * expected_mutations / 2);
    #endif // FIX_ISSUE_140

}

BOOST_AUTO_TEST_CASE(test_jobo_calc_chance_dead_kids)
{
    // Test calc_chance_dead_kids
    const genotype w("abCd");
    const genotype q("AbCd");
    const genotype s("aBcD");
    double chance_dead_kids ;
    chance_dead_kids = calc_chance_dead_kids(w,q);
    BOOST_CHECK_EQUAL(chance_dead_kids,-0.25);
    chance_dead_kids = calc_chance_dead_kids(q,s);
    BOOST_CHECK(chance_dead_kids ==(1));
}

BOOST_AUTO_TEST_CASE(test_jobo_get_unique_genotypes)
{
    vector<individual> first_population(10, individual("abcd"));
    const genotypes gs = get_unique_genotypes(first_population);
    const int n_genotypes{static_cast<int>(gs.size())};
    BOOST_CHECK(n_genotypes > 0);
}

BOOST_AUTO_TEST_CASE(test_jobo_count_good_species)
{
    // Test count_good_species
    {
      std::vector<genotype> first_population;
      genotype a{"abcd"};
      genotype b{"ABCD"};
      first_population.push_back(a);
      first_population.push_back(b);
      assert (first_population.size() != 0);
      int n_good_species = count_good_species(first_population);
      BOOST_CHECK_EQUAL (n_good_species,2);
    }
    {
      std::vector<genotype> first_population;
      genotype a{"Abcd"};
      genotype b{"AbCd"};
      genotype c{"abCd"};
      genotype d{"abcd"};
      genotype e{"ABCd"};
      genotype f{"ABCD"};
      first_population.push_back(a);
      first_population.push_back(b);
      first_population.push_back(c);
      first_population.push_back(d);
      first_population.push_back(e);
      first_population.push_back(f);
      assert (first_population.size() != 0);
      assert (first_population.size() == 6);
      assert (first_population.size() != 8);
      int n_good_species = count_good_species(first_population);
      BOOST_CHECK_EQUAL (n_good_species,1);
    }
    {
      std::vector<genotype> first_population;
      genotype a{"Abcd"};
      genotype b{"AbCd"};
      genotype c{"abCd"};
      genotype d{"ABCd"};
      genotype e{"ABCD"};
      first_population.push_back(a);
      first_population.push_back(b);
      first_population.push_back(c);
      first_population.push_back(d);
      first_population.push_back(e);
      assert (first_population.size() != 0);
      assert (first_population.size() == 5);
      int n_good_species = count_good_species(first_population);
      BOOST_CHECK_EQUAL (n_good_species,1);
    }
    {
      std::vector<genotype> first_population;
      genotype a{"ABCD"};
      genotype b{"abCd"};
      genotype c{"Abcd"};
      genotype d{"abcd"};
      first_population.push_back(a);
      first_population.push_back(b);
      first_population.push_back(c);
      first_population.push_back(d);
      assert (first_population.size() != 0);
      assert (first_population.size() == 4);
      int n_good_species = count_good_species(first_population);
      BOOST_CHECK_EQUAL (n_good_species,2);
    }

    for (int i=0; i!=10; ++i)
    {
      std::vector<genotype> gs{create_test_genotypes_1()};
      int n_genotypes{static_cast<int>(gs.size())};
      int n_good_species = count_good_species(gs);
      BOOST_CHECK (n_good_species <= n_genotypes);
    }
}

BOOST_AUTO_TEST_CASE(test_jobo_collect_viable_genotypes)
{
  {
    std::vector<individual> first_population;
    individual a{"ABCD"};
    individual b{"abCd"};
    individual c{"Abcd"};
    individual d{"abcD"};
    first_population.push_back(a);
    first_population.push_back(b);
    first_population.push_back(c);
    first_population.push_back(d);
    assert (first_population.size() != 0);
    assert (first_population.size() == 4);
    std::vector<genotype> viable_population{collect_viable_genotypes(first_population)};
    const int n_viable_genotypes{static_cast<int>(viable_population.size())};
    BOOST_CHECK_EQUAL (n_viable_genotypes,3);
  }
}

BOOST_AUTO_TEST_CASE(test_jobo_count_possible_species)
{
    // Test count_possible_species
    for (int i=0; i!=10; ++i)
    {
      std::vector<individual> first_population;
      genotypes gs = create_test_genotypes_1();
      int n_genotypes{static_cast<int>(gs.size())};
      int n_possible_species = count_possible_species(first_population);
      BOOST_CHECK (n_possible_species <= n_genotypes);
    }

    {
      std::vector<individual> first_population;
      individual a{"Abcd"};
      individual b{"AbCd"};
      individual c{"abCd"};
      individual d{"abcd"};
      individual e{"aBCd"};
      first_population.push_back(a);
      first_population.push_back(b);
      first_population.push_back(c);
      first_population.push_back(d);
      first_population.push_back(e);
      int n_possible_species = count_possible_species(first_population);
      BOOST_CHECK_EQUAL (n_possible_species,2);
    }
    { std::vector<individual> first_population;
      individual a{"Abcd"};
      individual b{"abCd"};
      individual c{"aBCd"};
      first_population.push_back(a);
      first_population.push_back(b);
      first_population.push_back(c);
      int n_possible_species = count_possible_species(first_population);
      BOOST_CHECK_EQUAL (n_possible_species,2);
    }
    {
      std::vector<individual> first_population;
      individual a{"Abcd"};
      individual b{"AbCd"};
      individual c{"abCd"};
      individual d{"abcd"};
      individual e{"aBCd"};
      individual f{"aBcD"};
      first_population.push_back(a);
      first_population.push_back(b);
      first_population.push_back(c);
      first_population.push_back(d);
      first_population.push_back(e);
      first_population.push_back(f);
      int n_possible_species = count_possible_species(first_population);
      BOOST_CHECK_EQUAL (n_possible_species,3);
    }
    {
      std::vector<individual> first_population;
      individual a{"Abcd"};
      individual b{"AbCd"};
      individual c{"abCd"};
      individual d{"aBCd"};
      individual e{"aBcD"};
      first_population.push_back(a);
      first_population.push_back(b);
      first_population.push_back(c);
      first_population.push_back(d);
      first_population.push_back(e);
      int n_possible_species = count_possible_species(first_population);
      BOOST_CHECK_EQUAL (n_possible_species,3);
    }
    {
      std::vector<individual> first_population;
      individual a{"abcd"};
      individual b{"Abcd"};
      individual c{"AbCd"};
      individual d{"aBCd"};
      individual e{"aBcD"};
      first_population.push_back(a);
      first_population.push_back(b);
      first_population.push_back(c);
      first_population.push_back(d);
      first_population.push_back(e);
      int n_possible_species = count_possible_species(first_population);
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

    {
      const std::vector<individual> population =
      {
        individual("abcDefgH"),
        individual("AbcDeFGh"),
        individual("aBcDeFGh"),
        individual("abcdefGh"),
        individual("abcdefGh"),
        individual("aBcdefGh"),
        individual("abcDefGh"),
        individual("abCdefGh"),
        individual("aBCdefgh"),
        individual("abCdeFGh"),
        individual("AbCdEfgh"),
        individual("abCdefGh"),
        individual("abcdefGh"),
        individual("aBcDEfgh"),
        individual("abCdEfgH"),
        individual("aBcdefgH"),
        individual("aBcdefgH"),
        individual("aBcdefgh"),
        individual("abCdefGh"),
        individual("abCdeFGh"),
        individual("abcdefgH"),
        individual("aBcdefgh"),
        individual("abCdefGh"),
        individual("abCdEfGh"),
        individual("abCdefGh"),
        individual("aBcDEfgh")
     /* individual("aBcdefgH"),
        individual("abCdefgh"),
        individual("abCdeFGh"),
        individual("Abcdefgh"),
        individual("abCdEfgH")
        individual("aBcdeFgh"),
        individual("abCdEfgh")
        individual("aBcdefgh"),
        individual("AbcdefgH"),
        individual("aBCdefgh"),
        individual("abcDefGh"),
        individual("abcdEfgh"),
        individual("aBcdefgH"),
        individual("aBcDEfgH"),
        individual("abcdefgH")*/
        };
    int n_possible_species = count_possible_species(population);
    BOOST_CHECK (n_possible_species >= 0);
    }
    {
      const std::vector<individual> population =
      {
        individual("abcDefgH"),
        individual("AbcDeFGh"),
        individual("aBcDeFGh"),
        individual("abcdefGh"),
        individual("abcdefGh"),
        individual("aBcdefGh"),
        individual("abcDefGh"),
        individual("abCdefGh"),
        individual("aBCdefgh"),
        individual("abCdeFGh"),
        individual("AbCdEfgh"),
        individual("abCdefGh"),
        individual("abcdefGh"),
        individual("aBcDEfgh"),
        individual("abCdEfgH"),
        individual("aBcdefgH"),
        individual("aBcdefgH"),
        individual("aBcdefgh"),
        individual("abCdefGh"),
        individual("abCdeFGh"),
        individual("abcdefgH"),
        individual("aBcdefgh"),
        individual("abCdefGh"),
        individual("abCdEfGh"),
        individual("abCdefGh"),
        individual("aBcDEfgh"),
        individual("aBcdefgH"),
        individual("abCdefgh"),
        individual("abCdeFGh"),
        individual("Abcdefgh"),
        individual("abCdEfgH"),
        individual("aBcdeFgh"),
        individual("abCdEfgh")
    /*  individual("aBcdefgh"),
        individual("AbcdefgH"),
        individual("aBCdefgh"),
        individual("abcDefGh"),
        individual("abcdEfgh"),
        individual("aBcdefgH"),
        individual("aBcDEfgH"),
        individual("abcdefgH")*/
        };
    int n_possible_species = count_possible_species(population);
    BOOST_CHECK (n_possible_species >= 0);
    }
    #endif // FIX_ISSUE_58
}

BOOST_AUTO_TEST_CASE(test_jobo_for_create_test_population_1)
{
    // Test create_test_population function 1
    genotypes gs = create_test_genotypes_1();
    BOOST_CHECK_EQUAL(gs.size(), 4);
}

BOOST_AUTO_TEST_CASE(test_jobo_for_inviable_species_being_present)
{
    // Test for inviable species being present
    const genotypes gs = create_test_genotypes_1();
    BOOST_CHECK_EQUAL(get_n_unviable_species(gs), 1);
}

BOOST_AUTO_TEST_CASE(test_jobo_convert_ltt_to_nltt)
{
  {
    const std::vector <int> p = { 1, 2, 2};
    const pbd::nltt measured = convert_ltt_to_nltt(p);
    pbd::nltt expected;
    expected.add_timepoint(0.0, 0.5);
    expected.add_timepoint(0.5, 1.0);
    expected.add_timepoint(1.0, 1.0);
    BOOST_REQUIRE_EQUAL(measured.size(), expected.size());
    BOOST_CHECK(measured.get() == expected.get());
  }
  {
    const std::vector <int> q = { 2, 3, 4, 5, 6};
    const pbd::nltt measured = convert_ltt_to_nltt(q);

    pbd::nltt expected;
    expected.add_timepoint(0.0,  1.0 / 3.0);
    expected.add_timepoint(0.25, 1.0 / 2.0);
    expected.add_timepoint(0.5,  2.0 / 3.0);
    expected.add_timepoint(0.75, 5.0 / 6.0);
    expected.add_timepoint(1.0,  1.0 / 1.0);
    BOOST_REQUIRE_EQUAL(measured.size(), expected.size());
    BOOST_CHECK(measured.get() == expected.get());
  }
}

BOOST_AUTO_TEST_CASE(test_jobo_jkr_adapters_save_nltt_plot_should_produce_a_file)
{
  const parameters p = create_test_parameters_1();

  //Ensure there is no output file yet
  if (is_regular_file(get_nltt_plot_filename(p)))
  {
    delete_file_2(get_nltt_plot_filename(p));
  }
  assert(!is_regular_file(get_nltt_plot_filename(p)));
  const parameters a = create_test_parameters_1();
  jobo::simulation s(a);
  s.run();

  BOOST_CHECK(is_regular_file(get_nltt_plot_filename(p)));

  //Clean up
  delete_file_2(get_nltt_plot_filename(p));
}

BOOST_AUTO_TEST_CASE(test_jobo_jkr_adapters_save_nltt_plot_should_produce_a_file_with_content)
{
  const parameters p = create_test_parameters_1();

  //Ensure there is no output file yet
  if (is_regular_file(get_nltt_plot_filename(p)))
  {
    delete_file_2(get_nltt_plot_filename(p));
  }
  assert(!is_regular_file(get_nltt_plot_filename(p)));

  const parameters a = create_test_parameters_1();
  jobo::simulation s(a);
  s.run();

  assert(is_regular_file(get_nltt_plot_filename(p)));
  const std::vector<std::string> text = file_to_vector(get_nltt_plot_filename(p));
  assert(text.size() == 1);
  const std::vector<std::string> words = seperate_string(text[0], ',');
  BOOST_CHECK_EQUAL(words.size(),p.get_generations());

  //Clean up
  delete_file_2(get_nltt_plot_filename(p));
}













#pragma GCC diagnostic pop
