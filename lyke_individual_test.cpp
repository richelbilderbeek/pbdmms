#include <cassert>
#include <iostream>
#include <fstream>
#include "lyke_random.h"
#include "lyke_individual.h"
#include "lyke_simulation.h"
#include "lyke_utils.h"
#include "pbd_helper.h"
#include <QFile>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(lyke_default_constructed_individuals_are_identical)
{
  #ifdef REALLY_INTERESTED
  const Individual a;
  const Individual b;
  BOOST_CHECK_EQUAL(a, b);
  #endif // REALLY_INTERESTED
}

BOOST_AUTO_TEST_CASE(lyke_default_copied_individuals_are_equal)
{
  Individual a;
  a.ugly();
  Individual b(a);
  BOOST_CHECK_EQUAL(a, b);
}

BOOST_AUTO_TEST_CASE(lyke_calculate_fertilization_efficiency)
{
  #ifdef REALLY_INTERESTED
  //Clones attract
  {
    const Individual a;
    const Individual b(a);
    BOOST_CHECK_EQUAL(calculate_fertilization_efficiency(a,b), 1.0);
    BOOST_CHECK_EQUAL(calculate_fertilization_efficiency(b,a), 1.0);
  }
  #endif // REALLY_INTERESTED
}

BOOST_AUTO_TEST_CASE(lyke_create_opposite)
{
  #ifdef REALLY_INTERESTED
  //Opposites think they are ugly
  const Individual a;
  const Individual b = create_opposite(a);
  BOOST_CHECK(calculate_fertilization_efficiency(a,b) < 0.01);
  BOOST_CHECK(calculate_fertilization_efficiency(b,a) < 0.01);
  #endif // REALLY_INTERESTED
}

BOOST_AUTO_TEST_CASE(lyke_ugly_ugly_individuals_are_not_ugly)
{
  #ifdef REALLY_INTERESTED
  Individual a;
  a.ugly();
  Individual b(a);
  assert(a == b);
  assert(b.match(&a) == 1.0); //Ugly individuals match
  //Now make b ugly agin
  b.ugly();
  BOOST_CHECK(b.match(&a) < 0.99);
  #endif // REALLY_INTERESTED
}

BOOST_AUTO_TEST_CASE(lyke_individual_stream_out_operator)
{
  #ifdef REALLY_INTERESTED
  const Individual i;
  std::stringstream s;
  s << i;
  BOOST_CHECK(!s.str().empty());
  BOOST_CHECK(s.str() != "STUB");
  #endif //REALLY_INTERESTED
}

BOOST_AUTO_TEST_CASE(lyke_calculate_fertilization_efficiency_from_bitsets)
{
  {
    //Mask removes the synonymous (that is, unimportant) loci
    // Egg   : 00000000
    // Sperm : 00001111
    // Mask  : 01010101
    // Result: 01010000 it's a match if egg[i] == sperm[i] AND mask[i] == 0 (is not ignored)
    const boost::dynamic_bitset<> egg_loci(  8, 0b00000000);
    const boost::dynamic_bitset<> sperm_loci(8, 0b00001111);
    const boost::dynamic_bitset<> mask      (8, 0b01010101);
    const double decay_rate_per_mismatch{1.0};

    //I expect one match, thus a value of std::exp(-1)
    const double n_expected_matches{2.0};
    const double expected{std::exp(-decay_rate_per_mismatch * n_expected_matches)};
    const double measured{
      calculate_fertilization_efficiency(
        egg_loci,
        sperm_loci,
        decay_rate_per_mismatch,
        mask
      )
    };
    BOOST_CHECK_EQUAL(expected, measured);
  }
  {
    //Mask removes the synonymous (that is, unimportant) loci
    // Egg   : 000
    // Sperm : 000
    // Mask  : 011
    // Result: 100
    const boost::dynamic_bitset<> egg_loci(  8, 0b000);
    const boost::dynamic_bitset<> sperm_loci(8, 0b000);
    const boost::dynamic_bitset<> mask      (8, 0b011);
    const double decay_rate_per_mismatch{1.0};

    //I expect one match, thus a value of std::exp(-1)
    const double n_expected_matches{0.0};
    const double expected{std::exp(-decay_rate_per_mismatch * n_expected_matches)};
    const double measured{
      calculate_fertilization_efficiency(
        egg_loci,
        sperm_loci,
        decay_rate_per_mismatch,
        mask
      )
    };
    BOOST_CHECK_EQUAL(expected, measured);
  }
}



#pragma GCC diagnostic pop


