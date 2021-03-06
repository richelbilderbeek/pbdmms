#include "ribi_parameters.h"
#include <fstream>
#include <iostream>
#include "ribi_helper.h"
#include "pbd_helper.h"
#include "is_regular_file.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace ribi;

BOOST_AUTO_TEST_CASE(ribi_parameters_comparison)
{
  const auto a = create_test_parameters_1();
  const auto b = create_test_parameters_1();
  const auto c = create_test_parameters_2();
  BOOST_CHECK(a == a);
  BOOST_CHECK(a == b);
  BOOST_CHECK(a != c);
  BOOST_CHECK(b == a);
  BOOST_CHECK(b == b);
  BOOST_CHECK(b != c);
  BOOST_CHECK(c != a);
  BOOST_CHECK(c != b);
  BOOST_CHECK(c == c);
}

BOOST_AUTO_TEST_CASE(ribi_parameters_streaming)
{
  const auto a = create_test_parameters_1();
  std::stringstream s;
  s << a;
  auto b = create_test_parameters_2();
  assert(a != b);
  s >> b;
  BOOST_CHECK(a == b);
}

BOOST_AUTO_TEST_CASE(ribi_load_parameters_on_absent_file)
{
  const std::string filename{"test_ribi_load_parameters_on_absent_file"};
  BOOST_CHECK_THROW(
    load_parameters(filename),
    std::invalid_argument
  );
}


BOOST_AUTO_TEST_CASE(ribi_parameters_save_and_load)
{
  const std::string filename{"test_ribi_parameters_save_and_load"};
  const auto a = create_test_parameters_2();
  save_parameters(a, filename);
  const auto b = load_parameters(filename);
  BOOST_CHECK(a == b);

  pbd::delete_file(filename);
  assert(!is_regular_file(filename));
}

BOOST_AUTO_TEST_CASE(ribi_parameters_abuse)
{
  const int max_genetic_distance{1};
  const int n_generations{10};
  const std::size_t n_pin_loci{2};
  const std::size_t n_sil_loci{2};
  const probability pin_mutation_rate{0.1};
  const int population_size{10};
  const std::string results_genotype_frequency_graph_filename{"tmp.dot"};
  const int rng_seed{42};
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
      "filename with spaces.dot", //results_genotype_frequency_graph_filename,
      rng_seed,
      sil_mutation_rate
    ),
    std::invalid_argument
  );

  //sil_mutation_rate cannot be negative
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
      -1.0 //sil_mutation_rate
    ),
    std::invalid_argument
  );
  //sil_mutation_rate cannot be bigger than 1.0
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
      10.0 //sil_mutation_rate
    ),
    std::invalid_argument
  );
}

#pragma GCC diagnostic pop
