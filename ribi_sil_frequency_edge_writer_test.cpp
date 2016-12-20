#include "ribi_sil_frequency_edge_writer.h"

#include "ribi_sil_frequency_phylogeny.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace ribi;

BOOST_AUTO_TEST_CASE(ribi_sil_frequency_edge_writer_to_stream)
{
  const auto g = get_test_sil_frequency_phylogeny_1();
  std::stringstream s;
  s << g;
  BOOST_CHECK(!s.str().empty());

}

#pragma GCC diagnostic pop


