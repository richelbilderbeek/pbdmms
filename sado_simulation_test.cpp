#include "sado_simulation.h"

#include <fstream>

#include "sado_helper.h"
#include "sado_parameters.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace sado;

BOOST_AUTO_TEST_CASE(sado_simulation_construction)
{
  //Must be there sometimes.. :-(
  //Uncomment if sado_simulation_must_reproduce_golden_standard fails
  //(yes, that is stupid, but do not forget that the sado RNG uses a global)
  const auto p = create_golden_standard_parameters();
  simulation s(p);
  s.do_timestep();
}

BOOST_AUTO_TEST_CASE(sado_simulation_must_reproduce_golden_standard)
{
  //This test takes too long and therefore, it is only run on Travis CI
  if (!is_travis()) return;

  const std::string filename{
      "sado_simulation_must_reproduce_golden_standard.txt"};
  const auto p = create_golden_standard_parameters();
  simulation s(p);
  s.run();
  {
    std::ofstream f(filename);
    f << s.get_results();
  }
  auto measured_lines = file_to_vector(filename);
  assert(measured_lines.back() == "");
  measured_lines.pop_back();
  const auto expected_lines = get_golden_output();
  BOOST_REQUIRE_EQUAL(measured_lines.size(), expected_lines.size());

  const auto n_lines = measured_lines.size();
  for (auto i = 1u; i != n_lines; ++i) // Skip the header
  {
    const auto expected = to_doubles(seperate_string(expected_lines[i], ','));
    const auto measured = to_doubles(seperate_string(measured_lines[i], ','));
    BOOST_CHECK(is_more_or_less_same(expected, measured));
  }
}

BOOST_AUTO_TEST_CASE(sado_simulation_must_detect_a_population_of_one)
{
  auto p = create_golden_standard_parameters();
  p.set_pop_size(1);
  BOOST_CHECK_THROW(
    simulation s(p),
    std::invalid_argument
  );
}


#pragma GCC diagnostic pop
