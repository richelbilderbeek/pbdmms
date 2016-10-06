#include "distancer_simulation.h"
#include <boost/test/unit_test.hpp>

#include "distancer_simulation.h"
#include <exception>
#include <iostream>
#include <fstream>
#include "distancer_helper.h"
#include "is_regular_file.h"
#include "convert_dot_to_svg.h"
#include "convert_svg_to_png.h"

BOOST_AUTO_TEST_CASE(test_do_simulation_simple_run)
{
  const int max_genetic_distance{1};
  const int n_generations{10};
  const int n_pin_loci{1};
  const int n_sil_loci{4};
  const double pin_mutation_rate{0.1}; //Chance to have 1 locus flipped in a genome
  const int population_size{8};
  const std::string results_genotype_frequency_graph_filename{"test_do_simulation_simple_run.dot"};
  const int rng_seed{42};
  const int sampling_interval{1};
  const double sil_mutation_rate{0.25}; //Chance to have 1 locus flipped in a genome
  const parameters p(
    max_genetic_distance,
    n_generations,
    n_pin_loci,
    n_sil_loci,
    pin_mutation_rate,
    population_size,
    results_genotype_frequency_graph_filename,
    rng_seed,
    sampling_interval,
    sil_mutation_rate
  );

  BOOST_TEST_PASSPOINT();
  do_simulation(p); //Will abort the test if throws
  BOOST_TEST_PASSPOINT();
  BOOST_REQUIRE(is_regular_file(results_genotype_frequency_graph_filename));
  convert_dot_to_svg(results_genotype_frequency_graph_filename, "test_do_simulation_simple_run.svg");
  convert_svg_to_png("test_do_simulation_simple_run.svg", "test_do_simulation_simple_run.png");
}

BOOST_AUTO_TEST_CASE(test_do_simulation_run_example_sim)
{
  const int max_genetic_distance{1};
  const int n_generations{1650};
  const int n_pin_loci{5};
  const int n_sil_loci{4};
  const double pin_mutation_rate{0.1}; //Chance to have 1 locus flipped in a genome
  const int population_size{8};
  const std::string results_genotype_frequency_graph_filename{"test_do_simulation_run_example_sim.dot"};
  const int rng_seed{30};
  const int sampling_interval{150};
  const double sil_mutation_rate{0.1}; //Chance to have 1 locus flipped in a genome
  const parameters p(
    max_genetic_distance,
    n_generations,
    n_pin_loci,
    n_sil_loci,
    pin_mutation_rate,
    population_size,
    results_genotype_frequency_graph_filename,
    rng_seed,
    sampling_interval,
    sil_mutation_rate
  );
  BOOST_TEST_PASSPOINT();
  do_simulation(p);
  BOOST_TEST_PASSPOINT();
  BOOST_CHECK(is_regular_file(results_genotype_frequency_graph_filename));
  convert_dot_to_svg(results_genotype_frequency_graph_filename, "test_do_simulation_run_example_sim.svg");
  convert_svg_to_png("test_do_simulation_run_example_sim.svg", "test_do_simulation_run_example_sim.png");
}
