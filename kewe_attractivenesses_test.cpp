#include "kewe_attractivenesses.h"

#include "kewe_parameters.h"
#include "kewe_individuals.h"
#include "kewe_helper.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace kewe;

BOOST_AUTO_TEST_CASE(kewe_calculate_attractiveness_precise)
{
  ///Creates a matrix of attractivenesses
  ///Lets say we have three individuals, a, b and c:
  /// +---+-------------------+------------+----------------+--------------+
  /// | # | female_preference | male_trait | female_ecotype | male_ecotype |
  /// +---+-------------------+------------+----------------+--------------+
  /// | a |       1.0         |    1.0     |       1.0      |      1.0     |
  /// | b |       1.0         |    2.0     |       1.0      |      1.0     |
  /// | c |       1.0         |    3.0     |       1.0      |      1.0     |
  /// +---+-------------------+------------+----------------+--------------+
  /// * mate_spec_mate = 1.0
  /// * mate_spec_eco = 1.0
  /// +-----+---------------------------+
  /// | dx  | gauss(dx, 1.0)            |
  /// +-----+---------------------------+
  /// | 0.0 | e(-0.0) = 1.0             |
  /// | 1.0 | e(-0.5) = 0.606530659713  |
  /// | 2.0 | e(-2.0) = 0.135335283237  |
  /// | 3.0 | e(-4.5) = 0.0111089965382 |
  /// +-----+---------------------------+
  ///Then the matrix will be:
  /// +---+---+----+----+
  /// |   | a |  b |  c |
  /// +---+---+----+----+
  /// | a |0.0|0.61|0.14|
  /// | b |1.0|0.0 |0.14|
  /// | c |1.0|0.61|0.0 |
  /// +---+---+----+----+
  const individuals pop = create_test_individuals_1();
  assert(pop.size() == 3);
  const gausser mate_spec_eco(1.0);
  const gausser mate_spec_mate{1.0};
  const attractivenesses as{
    calc_attractivenesses(
      pop,
      mate_spec_eco,
      mate_spec_mate
    )
  };
  BOOST_CHECK(is_valid(as));
  BOOST_CHECK(is_square(as));
  BOOST_CHECK(has_diagonal_of_zeroes(as));
  BOOST_REQUIRE(as.size() == 3);
  BOOST_CHECK_CLOSE(as[0][0] ,0.0           , 0.01);
  BOOST_CHECK_CLOSE(as[0][1] ,0.606530659713, 0.01);
  BOOST_CHECK_CLOSE(as[0][2] ,0.135335283237, 0.01);
  BOOST_CHECK_CLOSE(as[1][0] ,1.0           , 0.01);
  BOOST_CHECK_CLOSE(as[1][1] ,0.0           , 0.01);
  BOOST_CHECK_CLOSE(as[1][2] ,0.135335283237, 0.01);
  BOOST_CHECK_CLOSE(as[2][0] ,1.0           , 0.01);
  BOOST_CHECK_CLOSE(as[2][1] ,0.606530659713, 0.01);
  BOOST_CHECK_CLOSE(as[2][2] ,0.0           , 0.01);

}

BOOST_AUTO_TEST_CASE(kewe_results_test_calculate_attractiveness)
{
  const gausser mate_spec_eco(1.0);
  const gausser mate_spec_mate(1.0);

  const individual a( 0.5,  0.5,  0.5, { 0.5}, { 0.5}, { 0.5} );
  const individual b(-0.5, -0.5, -0.5, {-0.5}, {-0.5}, {-0.5} );

  BOOST_CHECK(a != b);

  const individuals pop = {a, a, b, b};

  const attractivenesses as = calc_attractivenesses(pop, mate_spec_eco, mate_spec_mate);

  /*
  0 should be attracted to 1, but not 2 and 3
  1 should be attracted to 0, but not 2 and 3
  2 should be attracted to 3, but not 0 and 1
  3 should be attracted to 2, but not 0 and 1
  */

  BOOST_CHECK(as[0][1] > as[0][2] && as[0][1] > as[0][2]);
  BOOST_CHECK(as[1][0] > as[1][2] && as[1][0] > as[1][3]);
  BOOST_CHECK(as[2][3] > as[2][0] && as[2][3] > as[2][1]);
  BOOST_CHECK(as[3][2] > as[3][0] && as[3][2] > as[3][1]);

}

#pragma GCC diagnostic pop

