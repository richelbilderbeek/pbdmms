#include "sado_results.h"
#include "sado_helper.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace sado;

BOOST_AUTO_TEST_CASE(sado_get_golden_standard_results)
{
  //const auto r = get_golden_standard_results();
}


BOOST_AUTO_TEST_CASE(sado_create_header)
{
  const auto p = create_golden_standard_parameters();
  create_header(p);
  const std::string filename = p.get_output_filename();
  const auto lines = file_to_vector(filename);
  BOOST_CHECK_EQUAL(lines.size(), 2);
  BOOST_CHECK_EQUAL(lines[1], std::string());
  const std::string measured{lines[0]};
  const std::string expected{get_golden_output()[0]};
  BOOST_CHECK_EQUAL(measured, expected);
}


#pragma GCC diagnostic pop


