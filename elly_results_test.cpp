#include "elly_results.h"

#include <vector>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace elly;


BOOST_AUTO_TEST_CASE(elly_find_youngest_parent)
{
  const double t_birth1{1.0};
  const double t_birth2{3.0};
  const double t_colonisation = 2.0;
  species a = create_new_test_species(t_birth1, location::mainland);
  a.migrate_to_island(t_colonisation);
  const species b = create_descendant(a, t_birth2, location::island);
  const std::vector<species> pop = {a, b};
  const species parent = find_youngest_parent(pop);

  BOOST_CHECK_EQUAL(a, parent);
}

BOOST_AUTO_TEST_CASE(elly_collect_kids)
{
  //Single species, thus zero kids
  {
    species a = create_new_test_species(location::mainland);
    const std::vector<species> population = {a};
    const std::vector<species> kids = collect_kids(a,population);
    BOOST_CHECK_EQUAL(kids.size(), 0);
  }
  //Two species, one with kids, on (as the kid) without
  // a father
  // |
  // b kid
  {
    const species a = create_new_test_species(location::mainland);
    const species b = create_descendant(a, 1.0, location::mainland);
    const std::vector<species> population = {a, b};
    const std::vector<species> kids_a = collect_kids(a,population);
    BOOST_CHECK_EQUAL(kids_a.size(), 1);
    const std::vector<species> kids_b = collect_kids(b,population);
    BOOST_CHECK_EQUAL(kids_b.size(), 0);
  }
  //Three species
  // a grandfather
  // |
  // b father
  // |
  // c kid
  {
    const species a = create_new_test_species(location::mainland);
    const species b = create_descendant(a, 1.0, location::mainland);
    const species c = create_descendant(b, 1.0, location::mainland);
    const std::vector<species> population = {a, b, c};
    const std::vector<species> kids_a = collect_kids(a,population);
    BOOST_CHECK_EQUAL(kids_a.size(), 2);
    const std::vector<species> kids_b = collect_kids(b,population);
    BOOST_CHECK_EQUAL(kids_b.size(), 1);
    const std::vector<species> kids_c = collect_kids(c,population);
    BOOST_CHECK_EQUAL(kids_c.size(), 0);
  }
  //Four species
  // a grandfather
  // |
  // b father
  // |
  // c kid
  //
  // d (unrelated)
  {
    const species a = create_new_test_species(location::mainland);
    const species b = create_descendant(a, 1.0, location::mainland);
    const species c = create_descendant(b, 1.0, location::mainland);
    const species d = create_new_test_species(location::mainland);
    const std::vector<species> population = {a, b, c};
    const std::vector<species> kids_a = collect_kids(a,population);
    BOOST_CHECK_EQUAL(kids_a.size(), 2);
    const std::vector<species> kids_b = collect_kids(b,population);
    BOOST_CHECK_EQUAL(kids_b.size(), 1);
    const std::vector<species> kids_c = collect_kids(c,population);
    BOOST_CHECK_EQUAL(kids_c.size(), 0);
    const std::vector<species> kids_d = collect_kids(d,population);
    BOOST_CHECK_EQUAL(kids_d.size(), 0);
  }
  //Three species
  //    a father
  //    |
  // +--+--+
  // |     |
  // b     c kids
  {
    const species a = create_new_test_species(location::mainland);
    const species b = create_descendant(a, 1.0, location::mainland);
    const species c = create_descendant(a, 2.0, location::mainland);
    const std::vector<species> population = {a, b, c};
    const std::vector<species> kids_a = collect_kids(a,population);
    BOOST_CHECK_EQUAL(kids_a.size(), 2);
    const std::vector<species> kids_b = collect_kids(b,population);
    BOOST_CHECK_EQUAL(kids_b.size(), 0);
    const std::vector<species> kids_c = collect_kids(c,population);
    BOOST_CHECK_EQUAL(kids_c.size(), 0);
  }
}



BOOST_AUTO_TEST_CASE(elly_collect_branching_times_for_non_endemic)
{
  species a = create_new_test_species(location::mainland);
  const double t_migrate{1.0};
  a.migrate_to_island(t_migrate);
  const std::vector<species> population = { a  };
  const std::vector<double> v = collect_branching_times(population);
  BOOST_REQUIRE_EQUAL(v.size(), 1);
  BOOST_CHECK_CLOSE(v[0], t_migrate, 0.00001);
}

BOOST_AUTO_TEST_CASE(elly_collect_branching_times_single_endemic)
{
  species a = create_new_test_species(location::mainland);
  const double t_migrate{1.0};
  const double t_anagenesis{2.0};
  a.migrate_to_island(t_migrate);
  const species b = create_descendant(a, t_anagenesis, location::island);
  const std::vector<species> population = { a, b };
  const std::vector<double> v = collect_branching_times(population);
  BOOST_REQUIRE_EQUAL(v.size(), 1);
  BOOST_CHECK_CLOSE(v[0], b.get_time_of_birth(), 0.0001);
}

BOOST_AUTO_TEST_CASE(elly_collect_branching_times_two_branches)
{
  const double t_migrate1{1.0};
  const double t_migrate2{2.0};
  const double t_diversify{1.5};
  species a = create_new_test_species(location::mainland);
  a.migrate_to_island(t_migrate1);
  const species b = create_descendant(a, t_diversify, location::island);
  species c = create_descendant(a, t_diversify, location::mainland);
  c.migrate_to_island(t_migrate2);
  const species d  = create_descendant(a, 0.0, location::mainland);
  const std::vector<species> population{a, b, c, d};
  const std::vector<double> v = collect_branching_times(population);
  BOOST_REQUIRE_EQUAL(v.size(), 2);
  BOOST_CHECK_CLOSE(v[0], b.get_time_of_birth(), 0.0001);
  BOOST_CHECK_CLOSE(v[1], c.get_time_of_birth(), 0.0001);
}

#pragma GCC diagnostic pop
