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
  const parameters p(10,42,0.5,10,6,0.05);
  jkr::do_experiment<
    jobo::parameters,
    jobo::simulation,
    jobo::results
  >(p);
}

BOOST_AUTO_TEST_CASE(test_jobo_create_next_population)
{
  const parameters p(10,42,0.5,10,6,0.05);
  simulation s = create_simulation(p);
  std::mt19937 rng_engine(get_rng_seed(p));
  vector <individual> test_population = create_next_population(s,rng_engine);
  BOOST_CHECK (test_population.size() > 0);
}

BOOST_AUTO_TEST_CASE(test_jobo_create_ltt_plot_filename)
{
  const parameters d(10,42,0.5,10,6,0.05);
  std::string filename = get_ltt_plot_filename(d);
  std::string e ("jobo abcdef,10,6,0.5,10,0.05,42");
  BOOST_CHECK(filename == e);
}

BOOST_AUTO_TEST_CASE(test_jobo_jkr_adapters_save_ltt_plot_should_produce_a_file)
{
  const parameters p(10,42,0.5,10,6,0.05);

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
  const parameters p(10,42,0.5,10,6,0.05);

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
  const std::vector<std::string> words = seperate_string(text[0], ' ');
  BOOST_CHECK_EQUAL(words.size(), p.get_generations());

  //Clean up
  delete_file(get_ltt_plot_filename(p));
}


#pragma GCC diagnostic pop


