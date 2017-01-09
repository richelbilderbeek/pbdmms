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

BOOST_AUTO_TEST_CASE(elly_get_youngest_colonist)
{
  //t1: species a born, on mainland
  //t2: species b born, on mainland
  //t3: species b colonizes the island
  //t4: species a colonizes the island
  //So, although species b is born later,
  //it does colonize the island first
  //thus is the youngest colonist
  const double t1{1.1};
  const double t2{2.2};
  const double t3{3.3};
  const double t4{4.4};
  species a = create_new_test_species(t1, location::mainland);
  species b = create_new_test_species(t2, location::mainland);
  b.migrate_to_island(t3);
  a.migrate_to_island(t4);
  const std::vector<species> colonists = {a, b};
  BOOST_CHECK_EQUAL(b, get_youngest_colonist(colonists));
}

BOOST_AUTO_TEST_CASE(elly_get_time_of_birth_children)
{
  const double t1{1.1};
  const double t2{2.2};
  const double t3{3.3};
  const double t4{4.4};
  const species a = create_new_test_species(t1, location::mainland);
  const species b = create_descendant(a, t2, location::mainland);
  const species c = create_descendant(b, t3, location::mainland);
  const species d = create_descendant(c, t4, location::mainland);
  const std::vector<double> ts_birth_a = { t2, t3, t4 };
  const std::vector<double> ts_birth_b = {     t3, t4 };
  const std::vector<double> ts_birth_c = {         t4 };
  const std::vector<double> ts_birth_d = {            };
  const clade this_clade( {a, b, c, d} );
  BOOST_CHECK(ts_birth_a == get_time_of_birth_children(a, this_clade));
  BOOST_CHECK(ts_birth_b == get_time_of_birth_children(b, this_clade));
  BOOST_CHECK(ts_birth_c == get_time_of_birth_children(c, this_clade));
  BOOST_CHECK(ts_birth_d == get_time_of_birth_children(d, this_clade));
}


#pragma GCC diagnostic pop







