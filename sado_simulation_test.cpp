#include "sado_simulation.h"

#include <fstream>

#include "sado_helper.h"
#include "sado_parameters.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace sado;

BOOST_AUTO_TEST_CASE(sado_simulation_must_reproduce_golden_standard)
{
  const std::string filename{"sado_simulation_must_reproduce_golden_standard.txt"};
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

  const int n_lines{static_cast<int>(measured_lines.size())};
  for (int i=1; i!=n_lines; ++i) //Skip the header
  {
    const auto expected = to_doubles(seperate_string(expected_lines[i], ','));
    const auto measured = to_doubles(seperate_string(measured_lines[i], ','));
    BOOST_CHECK(is_more_or_less_same(expected, measured));
  }

  /*


  if (measured != expected)
  {
    std::cerr << "ERROR\n";
    const int sz{static_cast<int>(std::min(measured.size(), expected.size()))};
    for (int i=0; i!=sz; ++i)
    {
      std::cerr
        << "expected[" << i << "]:" << '\n'
        << expected[i] << '\n'
        << "measured[" << i << "]:" << '\n'
        << measured[i] << '\n'
      ;
    }

  }
  BOOST_CHECK(measured == expected);
  */
}

#pragma GCC diagnostic pop

