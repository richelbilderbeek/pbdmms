#include "distancer_parameters.h"
#include <fstream>
#include <iostream>
#include <boost/test/unit_test.hpp>
#include "distancer_helper.h"

BOOST_AUTO_TEST_CASE(test_parameters)
{
  const int max_genetic_distance{1};
  const int n_generations{10};
  const std::size_t n_pin_loci{2};
  const std::size_t n_sil_loci{2};
  const double pin_mutation_rate{0.1};
  const int population_size{10};
  const std::string results_genotype_frequency_graph_filename{"tmp.dot"};
  const int rng_seed{42};
  const int sampling_interval{1};
  const double sil_mutation_rate{0.1};

  BOOST_CHECK_NO_THROW(
    parameters(
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
    )
  );

  BOOST_CHECK_THROW(
    parameters(
      -1, //max_genetic_distance,
      n_generations,
      n_pin_loci,
      n_sil_loci,
      pin_mutation_rate,
      population_size,
      results_genotype_frequency_graph_filename,
      rng_seed,
      sampling_interval,
      sil_mutation_rate
    ),
    std::invalid_argument
  );


  BOOST_CHECK_THROW(
    parameters(
      max_genetic_distance,
      -1, //n_generations,
      n_pin_loci,
      n_sil_loci,
      pin_mutation_rate,
      population_size,
      results_genotype_frequency_graph_filename,
      rng_seed,
      sampling_interval,
      sil_mutation_rate
    ),
    std::invalid_argument
  );
  BOOST_CHECK_NO_THROW(
    parameters(
      max_genetic_distance,
      n_generations,
      -1, //n_loci, gets implicitly converted to unsigned integer
      -1, //same here
      pin_mutation_rate,
      population_size,
      results_genotype_frequency_graph_filename,
      rng_seed,
      sampling_interval,
      sil_mutation_rate
    )
  );

  BOOST_CHECK_THROW(
    parameters(
      max_genetic_distance,
      n_generations,
      n_pin_loci,
      n_sil_loci,
      -0.1, //pin_mutation_rate,
      population_size,
      results_genotype_frequency_graph_filename,
      rng_seed,
      sampling_interval,
      sil_mutation_rate
    ),
    std::invalid_argument
  );
  BOOST_CHECK_THROW(
    parameters(
      max_genetic_distance,
      n_generations,
      n_pin_loci,
      n_sil_loci,
      1.1, //pin_mutation_rate,
      population_size,
      results_genotype_frequency_graph_filename,
      rng_seed,
      sampling_interval,
      sil_mutation_rate
    ),
    std::invalid_argument
  );
  BOOST_CHECK_THROW(
    parameters(
      max_genetic_distance,
      n_generations,
      n_pin_loci,
      n_sil_loci,
      pin_mutation_rate,
      -1, //population_size,
      results_genotype_frequency_graph_filename,
      rng_seed,
      sampling_interval,
      sil_mutation_rate
    ),
    std::invalid_argument
  );
  BOOST_CHECK_THROW(
    parameters(
      max_genetic_distance,
      n_generations,
      n_pin_loci,
      n_sil_loci,
      pin_mutation_rate,
      population_size,
      "", //results_genotype_frequency_graph_filename,
      rng_seed,
      sampling_interval,
      sil_mutation_rate
    ),
    std::invalid_argument
  );
  BOOST_CHECK_THROW(
    parameters(
      max_genetic_distance,
      n_generations,
      n_pin_loci,
      n_sil_loci,
      pin_mutation_rate,
      population_size,
      results_genotype_frequency_graph_filename,
      rng_seed,
      -1, //sampling_interval
      sil_mutation_rate
    ),
    std::invalid_argument
  );
  //Cannot sample less often than the number of generations
  BOOST_CHECK_THROW(
    parameters(
      max_genetic_distance,
      10, //n_generations,
      n_pin_loci,
      n_sil_loci,
      pin_mutation_rate,
      population_size,
      results_genotype_frequency_graph_filename,
      rng_seed,
      1000, //sampling_interval
      sil_mutation_rate
    ),
    std::invalid_argument
  );
}
