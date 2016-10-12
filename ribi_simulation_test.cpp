#include "ribi_simulation.h"

#include "ribi_simulation.h"
#include <exception>
#include <iostream>
#include <fstream>
#include "ribi_helper.h"
#include "is_regular_file.h"
#include "convert_dot_to_svg.h"
#include "convert_svg_to_png.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace ribi;

BOOST_AUTO_TEST_CASE(test_simulation_constructor)
{
  const parameters p = create_test_parameters_1();
  const simulation s(p);
  BOOST_CHECK(s.get_parameters() == p);
}

BOOST_AUTO_TEST_CASE(test_simulation_first_generation_is_generation_zero)
{
  const parameters p = create_test_parameters_1();
  const simulation s(p);
  BOOST_CHECK(s.get_current_generation() == 0);
}


BOOST_AUTO_TEST_CASE(test_do_simulation_simple_run)
{
  const parameters p = create_test_parameters_1();
  BOOST_TEST_PASSPOINT();
  do_simulation(p); //Will abort the test if throws
  BOOST_TEST_PASSPOINT();
  BOOST_REQUIRE(is_regular_file(p.get_filename_genotype_frequency_graph()));
  convert_dot_to_svg(p.get_filename_genotype_frequency_graph(), "test_do_simulation_simple_run.svg");
  convert_svg_to_png("test_do_simulation_simple_run.svg", "test_do_simulation_simple_run.png");
}

BOOST_AUTO_TEST_CASE(test_do_simulation_run_example_sim)
{
  const parameters p = create_test_parameters_2();
  BOOST_TEST_PASSPOINT();
  do_simulation(p);
  BOOST_TEST_PASSPOINT();
  BOOST_CHECK(is_regular_file(p.get_filename_genotype_frequency_graph()));
  convert_dot_to_svg(p.get_filename_genotype_frequency_graph(), "test_do_simulation_run_example_sim.svg");
  convert_svg_to_png("test_do_simulation_run_example_sim.svg", "test_do_simulation_run_example_sim.png");
}


BOOST_AUTO_TEST_CASE(test_do_simulation_cpp_simple_run)
{
  const parameters p = create_test_parameters_1();
  BOOST_TEST_PASSPOINT();
  do_simulation_cpp(
    p.get_max_genetic_distance(),
    p.get_n_generations(),
    p.get_n_pin_loci(),
    p.get_n_sil_loci(),
    p.get_pin_mutation_rate(),
    p.get_population_size(),
    p.get_filename_genotype_frequency_graph(),
    p.get_rng_seed(),
    p.get_sampling_interval(),
    p.get_sil_mutation_rate()
  );
  BOOST_TEST_PASSPOINT();
  BOOST_REQUIRE(is_regular_file(p.get_filename_genotype_frequency_graph()));
  convert_dot_to_svg(p.get_filename_genotype_frequency_graph(), "test_do_simulation_cpp_simple_run.svg");
  convert_svg_to_png("test_do_simulation_cpp_simple_run.svg", "test_do_simulation_cpp_simple_run.png");
}


#pragma GCC diagnostic pop
