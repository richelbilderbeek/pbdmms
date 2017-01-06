#include "elly_clade.h"

#include "elly_species.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace elly;

BOOST_AUTO_TEST_CASE(elly_all_have_same_clade_id)
{
  //Create two clades:
  // a -- b -- c
  //
  // d
  const species a = create_new_test_species(location::mainland);
  const species b = create_descendant(a, 1.0, location::mainland);
  const species c = create_descendant(b, 1.0, location::mainland);
  const species d = create_new_test_species(location::mainland);
  BOOST_CHECK(all_have_same_clade_id( {a, b, c} ));
  BOOST_CHECK(all_have_same_clade_id( {d} ));
  BOOST_CHECK(!all_have_same_clade_id( {a, d} ));
}

#pragma GCC diagnostic pop







