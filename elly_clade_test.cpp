#include "elly_clade.h"

#include "elly_species.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace elly;

BOOST_AUTO_TEST_CASE(elly_clade_construction)
{
  //Can create an empty clade
  {
    const std::vector<species> v = {};
    BOOST_CHECK_NO_THROW(clade c(v));
  }
  //Can create a clade with one species
  {
    const std::vector<species> v
      = { create_new_test_species(location::mainland) };
    BOOST_CHECK_NO_THROW(clade c(v));
  }
  //Can create a clade with two species of same clade ID
  {
    const species a = create_new_test_species(location::mainland);
    const species b = create_descendant(a, 1.0, location::mainland);
    const std::vector<species> v = { a, b };
    BOOST_CHECK_NO_THROW(clade c(v));
  }
  //Cannot create a clade with two species with different clade IDs
  {
    const species a = create_new_test_species(location::mainland);
    const species b = create_new_test_species(location::mainland);
    const std::vector<species> v = { a, b };
    assert(a.get_clade_id() != b.get_clade_id());
    BOOST_CHECK_THROW(clade c(v), std::invalid_argument);
  }
}

BOOST_AUTO_TEST_CASE(elly_all_have_same_clade_id)
{
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
  //Cannot measure when clade is empty
  {
    const std::vector<species> v = {};
    const clade c(v);
    BOOST_CHECK_THROW(c.get_id(), std::logic_error);
  }
}

BOOST_AUTO_TEST_CASE(elly_get_ancestor)
{
  const species a = create_new_test_species(location::mainland);
  const species b = create_descendant(a, 1.0, location::mainland);
  const std::vector<species> v = { a, b };
  const clade c(v);
  BOOST_CHECK(a == get_ancestor(b, c));
  BOOST_CHECK_THROW(get_ancestor(a, c), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(elly_has_ancestor)
{
  const species a = create_new_test_species(location::mainland);
  const species b = create_descendant(a, 1.0, location::mainland);
  const std::vector<species> v = { a, b };
  const clade c(v);
  BOOST_CHECK(!has_ancestor(a, c));
  BOOST_CHECK( has_ancestor(b, c));
}




#pragma GCC diagnostic pop







