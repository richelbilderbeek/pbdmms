#include "sado_erasure_method.h"

#include <fstream>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace sado;

BOOST_AUTO_TEST_CASE(sado_erasure_method_to_string_and_back)
{
  const std::vector<erasure_method> v = collect_all_erasure_methods();
  for (const auto e: v)
  {
    const std::string s = to_str(e);
    BOOST_CHECK_EQUAL(to_erasure_method(s), e);
  }
}

BOOST_AUTO_TEST_CASE(sado_erasure_method_to_out_stream)
{
  const std::vector<erasure_method> v = collect_all_erasure_methods();
  for (const auto e: v)
  {
    std::stringstream s;
    s << e;
    BOOST_CHECK(!s.str().empty());
  }
}

#pragma GCC diagnostic pop
