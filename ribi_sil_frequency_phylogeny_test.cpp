#include "ribi_sil_frequency_phylogeny.h"
#include "add_bundled_vertex.h"
#include "ribi_sil_frequency_vertex.h"
#include "ribi_individual.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace ribi;

BOOST_AUTO_TEST_CASE(test_ribi_get_test_sil_frequency_phylogeny_1)
{
  const auto g = get_test_sil_frequency_phylogeny_1();
  BOOST_CHECK_EQUAL(boost::num_vertices(g), 6);
  BOOST_CHECK_EQUAL(boost::num_edges(g), 6);
}


BOOST_AUTO_TEST_CASE(test_sil_frequency_phylogeny_one_vertex)
{
  sil_frequency_phylogeny g;
  BOOST_CHECK_EQUAL(boost::num_vertices(g), 0);

  const int frequency{42};
  const sil my_sil(2, 0b00);
  std::map<sil,int> sil_frequencies;
  sil_frequencies.insert(std::make_pair(my_sil, frequency));

  const int time{123};
  add_bundled_vertex(
    sil_frequency_vertex(
      sil_frequencies,
      time
    )
    , g
  );

  BOOST_CHECK_EQUAL(boost::num_vertices(g), 1);
  BOOST_CHECK(g[*vertices(g).first].get_sil_frequencies() == sil_frequencies);
}

#pragma GCC diagnostic pop
