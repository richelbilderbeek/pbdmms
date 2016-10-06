#include "distancer_sil_frequency_phylogeny.h"
#include <boost/test/unit_test.hpp>
#include "add_bundled_vertex.h"
#include "distancer_sil_frequency_vertex.h"
#include "distancer_individual.h"

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
