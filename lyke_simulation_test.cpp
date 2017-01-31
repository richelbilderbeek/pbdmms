#include <cassert>
#include <iostream>
#include <fstream>
#include "lyke_random.h"
#include "lyke_individual.h"
#include "lyke_simulation.h"
#include "lyke_utils.h"
#include "pbd_helper.h"
#include <QFile>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
  for (const auto& i: v) { os << i << '\n'; }
  return os;
}


BOOST_AUTO_TEST_CASE(lyke_default_constructed_individual_are_different)
{
    const Individual a;
    const Individual b;
    BOOST_CHECK(a != b);
    BOOST_CHECK(a.getEcotype() != b.getEcotype());
}

BOOST_AUTO_TEST_CASE(lyke_copied_individual_are_identical)
{
    const Individual a;
    const Individual b(a);
    BOOST_CHECK(a == b);
}

BOOST_AUTO_TEST_CASE(lyke_identical_individuals_have_a_higher_competition_than_different_ones)
{
    const Individual a;
    const Individual b(a);
    const Individual c; //Other
    assert(a == b);
    assert(a != c);
    const double between_copies = a.CalcCompetionIntensity(&b);
    const double between_different = a.CalcCompetionIntensity(&c);
    BOOST_CHECK(between_copies > between_different);
}

BOOST_AUTO_TEST_CASE(lyke_identical_individuals_can_mate_with_each_other)
{
    //#define FIX_ISSUE_62
    #ifdef FIX_ISSUE_62
    const Individual a;
    const Individual b(a);
    assert(a == b);
    BOOST_CHECK (a.match(&b) > 0.99);
    #endif //~FIX_ISSUE_62
}

BOOST_AUTO_TEST_CASE(lyke_different_individuals_have_a_lesser_probility)
{
    #ifdef FIX_ISSUE_62
    const Individual a;
    const Individual b (a);
    Individual c; //Other
    assert(a == b);
    assert(a != c);

    //std::cout << "checking if c is ugly.\n";
    //c.print();
    //std::cout << "checking if a is ugly.\n";
    //a.print();
    c.ugly();
    //c.print();
    const double p_mate_ab{a.match(&b)};
    const double p_mate_ac{a.match(&c)};

    BOOST_CHECK(p_mate_ab == p_mate_ac);
    #endif //~FIX_ISSUE_62
}

BOOST_AUTO_TEST_CASE(lyke_create_n_offspring_per_individual)
{
  /*
  {
    rnd::discrete_distribution rng;
    std::vector<int> expected_n_offspring = {1};
    BOOST_CHECK(create_n_offspring_per_individual(viabilities) == expected_n_offspring);
  }
  {
    rnd::discrete_distribution rng;
    std::vector<int> expected_n_offspring = {1,1};
    BOOST_CHECK(create_n_offspring_per_individual(viabilities) == expected_n_offspring);
  }
  {
    rnd::discrete_distribution rng;
    std::vector<int> expected_n_offspring = {1,0};
    BOOST_CHECK(create_n_offspring_per_individual(viabilities) == expected_n_offspring);
  }
  {
    rnd::discrete_distribution rng;
    rng.
    std::vector<int> expected_n_offspring = {1,0};
    BOOST_CHECK(create_n_offspring_per_individual(viabilities) == expected_n_offspring);
  }
  {
    std::vector<int> viabilities = {1,2,3,4};
    std::vector<int> expected_n_offspring = {1,2,3,4};
    BOOST_CHECK(create_n_offspring_per_individual(viabilities) == expected_n_offspring);
  }
  */
}

BOOST_AUTO_TEST_CASE(lyke_run_simulation_should_produce_same_output)
{
    #ifdef FIX_ISSUE_62
    std::ofstream EcoTypeFilestream ("ecotype.csv"); //opens excel file
    std::ofstream HistogramFilestream("Histogram.csv");//opens excel file
    std::ofstream DefaultresultsFiles ("lyke_defaultresults.csv");

    const int seed{42};
    rnd::set_seed(seed);
    std::vector<Individual> population(popSize);
    for (int i = 0; i < static_cast<int>(simulationruns); ++i)  //number of generations
    {
      //EcoTypeFilestream << 1 + i;
      const auto next_population = create_and_log_next_generation(
        population, EcoTypeFilestream, DefaultresultsFiles
      );
      population = next_population;
      doStatistics(population);
      doHistogram(population, i+1, HistogramFilestream);

    }

    //TEST: output should be same as golden output
    const std::string golden_standard_filename{"defaultresults"};
    recreate_defaultresults_output(golden_standard_filename);
    assert(pbd::is_regular_file("lyke_defaultresults.csv"));
    assert(pbd::is_regular_file(golden_standard_filename));
    const auto this_results = pbd::file_to_vector("lyke_defaultresults.csv");
    const auto golden_results = pbd::file_to_vector(golden_standard_filename);
    BOOST_CHECK(golden_results == this_results);
    #endif //FIX_ISSUE_62
}

#pragma GCC diagnostic pop

