#include "sado_next_generation_method.h"

#include <fstream>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace sado;

BOOST_AUTO_TEST_CASE(sado_next_generation_method_to_string_and_back)
{
  const std::vector<next_generation_method> v = collect_all_next_generation_methods();
  for (const auto e: v)
  {
    const std::string s = to_str(e);
    BOOST_CHECK_EQUAL(to_next_generation_method(s), e);
  }
}

BOOST_AUTO_TEST_CASE(sado_next_generation_method_to_out_stream)
{
  const std::vector<next_generation_method> v = collect_all_next_generation_methods();
  for (const auto e: v)
  {
    std::stringstream s;
    s << e;
    BOOST_CHECK(!s.str().empty());
  }
}

#pragma GCC diagnostic pop
