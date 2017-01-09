#include "elly_results.h"

#include <vector>
#include "elly_clade.h"
#include "elly_parameters.h"
#include "elly_simulation.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace elly;

BOOST_AUTO_TEST_CASE(elly_collect_ancestors)
{
  {
    //Three species
    // a grandfather
    // |
    // b father
    // |
    // c kid
    {
      species a = create_new_test_species(location::mainland);
      a.migrate_to_island(1.0);
      const species b = create_descendant(a, 2.0, location::island);
      const species c = create_descendant(b, 3.0, location::island);
      const std::vector<species> population = {a, b, c};
      const std::vector<species> ancestors = collect_colonists(population);
      BOOST_CHECK_EQUAL(ancestors.size(), 1);
      BOOST_CHECK(ancestors[0].get_time_of_colonization() != 0.0 );
    }
  }
  /*
   Four species
   time immigration a = 1.0
   time immigration b = 2.0
           b father    a non-endemic
           |           |
        +--+--+        |
        |     |        |
        c     d        a
   */
  {
    const double t_birth_descendants{3.0};
    const double colonisation_time_a{1.0};
    const double colonisation_time_b{2.0};
    species b = create_new_test_species(location::mainland);
    species a = create_descendant(b, 0.5, location::mainland);
    a.migrate_to_island(colonisation_time_a);
    b.migrate_to_island(colonisation_time_b);
    const species c = create_descendant(b, t_birth_descendants, location::island);
    const species d = create_descendant(b, t_birth_descendants, location::island);
    const std::vector<species> population = { a , b , c , d};
    const std::vector<species> ancestors = collect_colonists(population);
    const std::vector<species> kids_a = collect_kids(a, population);
    BOOST_CHECK_EQUAL(ancestors.size(), 2);
    BOOST_CHECK_EQUAL(kids_a.size(), 0);
  }
}

BOOST_AUTO_TEST_CASE(elly_find_youngest_parent)
{
  {
    //single species migrates from mainland to island and has one descendant
    const double t_birth1{1.0};
    const double t_birth2{3.0};
    const double t_colonisation = 2.0;
    species a = create_new_test_species(t_birth1, location::mainland);
    a.migrate_to_island(t_colonisation);
    const species b = create_descendant(a, t_birth2, location::island);
    const std::vector<species> pop = {a, b};
    const species ancestor = find_youngest_colonist(pop);

    BOOST_CHECK_EQUAL(a, ancestor);
  }
  {
    //two species migrate from mainland to island at different times,
    //one leaves descendant
    const double t_birth{1.0};
    const double t_birth2{2.0};
    const double t_birth_endemic{4.0};
    const double t_colonisation{2.0};
    const double t_colonisation2{3.0};
    species a = create_new_test_species(t_birth, location::mainland);
    species b = create_descendant(a, t_birth2, location::mainland);
    a.migrate_to_island(t_colonisation);
    species c = create_descendant(a, t_birth_endemic, location::island);
    b.migrate_to_island(t_colonisation2);
    const std::vector<species> pop = {a, b, c};
    const species ancestor = find_youngest_colonist(pop);

    BOOST_CHECK_EQUAL(ancestor , a);
  }
  {
    const double t_birth{1.0};
    const double t_birth2{2.0};
    const double t_birth_endemic{4.0};
    const double t_colonisation{2.0};
    const double t_colonisation2{3.0};
    species a = create_new_test_species(t_birth, location::mainland);
    species b = create_descendant(a, t_birth2, location::mainland);
    b.migrate_to_island(t_colonisation);
    species c = create_descendant(b, t_birth_endemic, location::island);
    a.migrate_to_island(t_colonisation2);
    const std::vector<species> pop = {a, b, c};
    const species ancestor = find_youngest_colonist(pop);

    BOOST_CHECK_EQUAL(ancestor , b);
  }
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
  const clade this_clade(population);
  const std::vector<double> v = collect_branching_times(this_clade);
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
  const clade this_clade(population);
  const std::vector<double> v = collect_branching_times(this_clade);
  BOOST_REQUIRE_EQUAL(v.size(), 1);
  BOOST_CHECK_CLOSE(v[0], b.get_time_of_birth(), 0.0001);
}

BOOST_AUTO_TEST_CASE(elly_collect_branching_times_two_branches)
{
  {
    /*
           a          d
           |          |
        +--+--+       |
        |     |       |
        b     c       d

        d stays on the mainland,
        a migrates and diversifies
     */
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
    const std::vector<double> v = collect_branching_times(clade(population));
    BOOST_REQUIRE_EQUAL(v.size(), 1);
    BOOST_CHECK_CLOSE(v[0], b.get_time_of_birth(), 0.0001);
    BOOST_CHECK_CLOSE(v[0], c.get_time_of_birth(), 0.0001);
  }
  {
    /*
       time immigration a = 3.0
       time immigration b = 1.0
          a        b
          |        |
       +--+--+     +
       |     |     |
       c     d     e

       a, and b are from the same clade on the mainland
       they both immgrate and form new species
     */
    const double colonisation_time_b{1.0};
    const double colonisation_time_a{3.0};
    const double time_diversification_a{4.0};
    const double time_diversification_b{5.0};
    species a = create_new_test_species(location::mainland);
    species b = create_descendant(a, 0.0, location::mainland);
    b.migrate_to_island(colonisation_time_b);
    a.migrate_to_island(colonisation_time_a);
    const species c = create_descendant(a, time_diversification_a, location::island);
    const species d = create_descendant(a, time_diversification_a, location::island);
    const species e = create_descendant(b, time_diversification_b, location::island);
    const std::vector<species> population = {a, b, c, d, e};
    const std::vector<double> branching_times = collect_branching_times(clade(population));
    BOOST_CHECK_EQUAL(branching_times.size(), 1);
    BOOST_CHECK_CLOSE(branching_times[0], e.get_time_of_birth(), 0.0001);
  }
}

#ifdef FIX_ISSUE_180
BOOST_AUTO_TEST_CASE(elly_convert_to_daisie_input_with_main_ext)
{
  const elly::parameters p = create_parameters_set2();
  simulation s(p);
  s.run();
  const auto simulation_results = get_results(s);
  const daic::input i = convert_to_daisie_input_with_main_ext(simulation_results);
  BOOST_CHECK(!is_empty(i));
}

BOOST_AUTO_TEST_CASE(elly_convert_to_daisie_input_without_main_ext)
{
  const elly::parameters p = create_parameters_set2();
  simulation s(p);
  s.run();
  const auto simulation_results = get_results(s);
  const daic::input i = convert_to_daisie_input_without_main_ext(simulation_results);
  BOOST_CHECK(!is_empty(i));
}
#endif // FIX_ISSUE_180



#pragma GCC diagnostic pop
