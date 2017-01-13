#include "jobo_jkr_adapters.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include <boost/test/unit_test.hpp>

#include "file_to_vector.h"
#include "is_regular_file.h"
#include "jkr_experiment.h"
#include "jobo_parameters.h"
#include "jobo_results.h"
#include "jobo_simulation.h"
#include "seperate_string.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

using namespace jobo;

//Short version, do not use
void delete_file(const std::string& filename)
{
  assert(is_regular_file(filename));
  std::remove(filename.c_str());
  assert(!is_regular_file(filename));
}


BOOST_AUTO_TEST_CASE(test_jobo_jkr_adapters_test)
{
  const parameters p = create_test_parameters_1();
  jkr::do_experiment<
    jobo::parameters,
    jobo::simulation,
    jobo::results
  >(p);
}

BOOST_AUTO_TEST_CASE(test_jobo_create_next_population)
{
  const parameters p = create_test_parameters_1();
  const simulation s = create_simulation(p);
  std::mt19937 rng_engine(get_rng_seed(p));
  const individuals next_population = create_next_population(s, rng_engine);
  BOOST_CHECK_EQUAL(p.get_population_size(), static_cast<int>(next_population.size()));
}

BOOST_AUTO_TEST_CASE(test_jobo_create_ltt_plot_filename)
{
  const parameters d = create_test_parameters_1();
  const std::string f1 = get_ltt_plot_filename(d);
  const std::string f2 = d.get_ltt_plot_filename_viables();

  BOOST_CHECK_EQUAL(f1, f2);
}

BOOST_AUTO_TEST_CASE(test_jobo_jkr_adapters_save_ltt_plot_should_produce_a_file)
{
  const parameters p = create_test_parameters_1();

  //Ensure there is no output file yet
  if (is_regular_file(get_ltt_plot_filename(p)))
  {
    delete_file(get_ltt_plot_filename(p));
  }
  assert(!is_regular_file(get_ltt_plot_filename(p)));

  jkr::do_experiment<
    jobo::parameters,
    jobo::simulation,
    jobo::results
  >(p);

  BOOST_CHECK(is_regular_file(get_ltt_plot_filename(p)));

  //Clean up
  delete_file(get_ltt_plot_filename(p));
}

BOOST_AUTO_TEST_CASE(test_jobo_jkr_adapters_save_ltt_plot_should_produce_a_file_with_content)
{
  const parameters p = create_test_parameters_1();

  //Ensure there is no output file yet
  if (is_regular_file(get_ltt_plot_filename(p)))
  {
    delete_file(get_ltt_plot_filename(p));
  }
  assert(!is_regular_file(get_ltt_plot_filename(p)));

  jkr::do_experiment<
    jobo::parameters,
    jobo::simulation,
    jobo::results
  >(p);

  assert(is_regular_file(get_ltt_plot_filename(p)));
  const std::vector<std::string> text = file_to_vector(get_ltt_plot_filename(p));
  assert(text.size() == 1);
  const std::vector<std::string> words = seperate_string(text[0], ',');
  BOOST_CHECK_EQUAL(words.size(),p.get_generations());

  //Clean up
  delete_file(get_ltt_plot_filename(p));
}

BOOST_AUTO_TEST_CASE(jobo_population_becomes_inviable)
{
  const int population_size{10};
  const int seed{42};
  const double mutation_rate{0.2};
  const int n_generations{100};
  const int n_loci{12};
  const  parameters p(
    population_size,
    seed,
    mutation_rate,
    n_generations,
    n_loci
  );

  const auto f = [&p]() {
      jkr::do_experiment<
      jobo::parameters,
      jobo::simulation,
      jobo::results
    >(p);
  };
  simulation s(p);
  BOOST_CHECK_THROW(f(), std::runtime_error);
}


#pragma GCC diagnostic pop


