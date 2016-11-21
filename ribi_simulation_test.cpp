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

BOOST_AUTO_TEST_CASE(test_simulation_second_generation_is_generation_one)
{
  const parameters p = create_test_parameters_1();
  simulation s(p);
  s.do_one_timestep();
  BOOST_CHECK(s.get_current_generation() == 1);
}


BOOST_AUTO_TEST_CASE(test_do_simulation_simple_run)
{
  const parameters p = create_test_parameters_1();
  do_simulation(p); //Will abort the test if throws
  BOOST_CHECK(is_regular_file(p.get_filename_genotype_frequency_graph()));
  std::remove(p.get_filename_genotype_frequency_graph().c_str());
  assert(!is_regular_file(p.get_filename_genotype_frequency_graph()));
  clean_simulation(p);
}

BOOST_AUTO_TEST_CASE(test_do_simulation_crash_41)
{
  #ifdef FIX_ISSUE_41
  const parameters p = create_test_parameters_2();
  do_simulation(p); //Will abort the test if throws
  BOOST_CHECK(is_regular_file(p.get_filename_genotype_frequency_graph()));
  std::remove(p.get_filename_genotype_frequency_graph().c_str());
  assert(!is_regular_file(p.get_filename_genotype_frequency_graph()));
  clean_simulation(p);
  #endif // FIX_ISSUE_41
}

BOOST_AUTO_TEST_CASE(test_do_simulation_cpp_simple_run)
{
  const parameters p = create_test_parameters_1();
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
  BOOST_CHECK(is_regular_file(p.get_filename_genotype_frequency_graph()));
  std::remove(p.get_filename_genotype_frequency_graph().c_str());
  assert(!is_regular_file(p.get_filename_genotype_frequency_graph()));
  clean_simulation(p);
}

BOOST_AUTO_TEST_CASE(test_ribi_find_parents_use)
{
  const individual mother(pin("ACGT"), create_sil("0101"));
  const individual father(pin("TGCA"), create_sil("1010"));
  assert(mother != father);
  const population pop = { mother, father };
  const int max_genetic_distance{1};
  std::mt19937 rng_engine;
  const auto p = find_parents(pop, max_genetic_distance, rng_engine);
  //BOOST_CHECK_NE(p.first, p.second); //This may be true
  BOOST_CHECK(p.first  == mother || p.first  == father);
  BOOST_CHECK(p.second == mother || p.second == father);
}

#pragma GCC diagnostic pop
