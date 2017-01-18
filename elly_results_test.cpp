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
      const double t_colonization{1.0};
      a.migrate_to_island(t_colonization);
      const species b = create_descendant(a, 2.0, location::island);
      const species c = create_descendant(b, 3.0, location::island);
      const std::vector<species> population = {a, b, c};
      const std::vector<species> ancestors = collect_colonists(population);
      BOOST_REQUIRE_EQUAL(ancestors.size(), 1);
      BOOST_REQUIRE_EQUAL(ancestors[0].get_times_of_colonization().size(), 1);
      BOOST_CHECK_EQUAL(ancestors[0].get_times_of_colonization().back(), t_colonization);
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

BOOST_AUTO_TEST_CASE(elly_find_youngest_colonist)
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
  {
    /*    Mainland      Island
             a
             |-   -   -   -a
             |             |
             |-   -   -   -|
             |             |
             a             a
     Second time of immigration should be used
    */
   species a = create_new_test_species(location::mainland);
   const double t_migrate{1.0};
   const double t_migrate2{2.0};
   a.migrate_to_island(t_migrate);
   a.migrate_to_island(t_migrate2);
   const std::vector<species> population = {a};
   const std::vector<double> measured_brts = collect_branching_times(clade(population));
   const std::vector<double> expected_brts = {2.0};
   BOOST_CHECK_EQUAL_COLLECTIONS(
         std::begin(measured_brts), std::end(measured_brts),
         std::begin(expected_brts), std::end(expected_brts)
         );
  }
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
  BOOST_REQUIRE_EQUAL(v.size(), 2);
  BOOST_REQUIRE_EQUAL(a.get_times_of_colonization().size(), 1);
  BOOST_CHECK_CLOSE(v[0], a.get_times_of_colonization().back(), 0.0001);
  BOOST_CHECK_CLOSE(v[1], b.get_time_of_birth(), 0.0001);
}

BOOST_AUTO_TEST_CASE(elly_collect_branching_times_two_branches_1)
{
  /*    Mainland            Island
      +----+
      d    a
      |    |-   -   -   -   -   -a
      |    +<anagenesis          +<anagenesis
      |    c                     b
      |    |-   -   -   -   c    |
      |    |                |    |
      d    c                c    b

      d stays on the mainland,
      a migrates and diversifies
      there should be one branching times, time of colonization for a, and time of birth
      of species b.
   */
  const double t_migrate1{1.0};
  const double t_diversify{2.0};
  const double t_migrate2{3.0};
  species a = create_new_test_species(location::mainland);
  a.migrate_to_island(t_migrate1);
  const species b = create_descendant(a, t_diversify, location::island);
  species c = create_descendant(a, t_diversify, location::mainland);
  c.migrate_to_island(t_migrate2);
  const species d  = create_descendant(a, 0.0, location::mainland);
  const std::vector<species> population{a, b, c, d};
  const std::vector<double> v = collect_branching_times(clade(population));
  BOOST_REQUIRE_EQUAL(v.size(), 2);
  BOOST_REQUIRE_EQUAL(a.get_times_of_colonization().size(), 1);
  BOOST_CHECK_CLOSE(v[0], a.get_times_of_colonization().back(), 0.0001);
  BOOST_CHECK_CLOSE(v[1], b.get_time_of_birth(), 0.0001);
  BOOST_CHECK_CLOSE(v[1], c.get_time_of_birth(), 0.0001);
}

BOOST_AUTO_TEST_CASE(elly_collect_branching_times_two_branches_2)
{
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
  BOOST_REQUIRE_EQUAL(branching_times.size(), 2);
  BOOST_CHECK_CLOSE(branching_times[0], colonisation_time_b, 0.0001);
  BOOST_CHECK_CLOSE(branching_times[1], e.get_time_of_birth(), 0.0001);
  }
  {
    /*   Three species
    time
    0     a
    |     |
    1     Immigration
    |     |
    2  +--+--+
    |  |     |
    3  |     |      a
    |  |     |      |
    4  b     c      a
    */
    const double t_colonization1{1.0};
    const double t_cladogenesis{2.0};
    const double t_colonization2{3.0};
    species a = create_new_test_species(location::mainland);
    a.migrate_to_island(t_colonization1);
    a.go_extinct(t_cladogenesis, location::island);
    species b = create_descendant(a, t_cladogenesis,location::island);
    species c = create_descendant(a, t_cladogenesis, location::island);
    a.migrate_to_island(t_colonization2);
    const std::vector<species> pop = {a,b,c};
    const std::vector<double> measured_brts = collect_branching_times(clade(pop));
    const std::vector<double> expected_brts = {1.0, 2.0};
    BOOST_CHECK_EQUAL_COLLECTIONS(
      std::begin(measured_brts), std::end(measured_brts),
      std::begin(expected_brts), std::end(expected_brts)
    );
  }
  {
    /*   Three species
    time
        Island     Mainland
    0     a
    |     |
    1     |-  -  -  - a
    |     |           |
    2     |-  -  -  - |
    |     |           |
    3  +--+--+        |
    |  |     |        |
    4  b     c        a
    */
    const double t_colonization1{1.0};
    const double t_cladogenesis{3.0};
    const double t_colonization2{2.0};
    species a = create_new_test_species(location::mainland);
    a.migrate_to_island(t_colonization1);
    a.migrate_to_island(t_colonization2);
    a.go_extinct(t_cladogenesis, location::island);
    species b = create_descendant(a, t_cladogenesis,location::island);
    species c = create_descendant(a, t_cladogenesis, location::island);
    const std::vector<species> pop = {a,b,c};
    const std::vector<double> measured_brts = collect_branching_times(clade(pop));
    const std::vector<double> expected_brts = {2.0, 3.0};
    BOOST_CHECK_EQUAL_COLLECTIONS(
      std::begin(measured_brts), std::end(measured_brts),
      std::begin(expected_brts), std::end(expected_brts)
    );
  }
}

BOOST_AUTO_TEST_CASE(elly_multiple_times_colonisation)
{
  {
  //Three species
  //    a
  //    |
  //    Immigration
  //    |
  // +--+--+
  // |     |
  // b     c
  const double colonisation_time1{1.0};
  const double time_of_cladogenesis{2.0};
  species a = create_new_test_species(location::mainland);
  a.migrate_to_island(colonisation_time1);
  a.go_extinct(time_of_cladogenesis, location::island);
  const species b = create_descendant(a, time_of_cladogenesis, location::island);
  const species c = create_descendant(a, time_of_cladogenesis, location::island);
  const std::vector<species> pop{a,b,c};
  BOOST_CHECK(!multiple_times_colonisation(pop));
  }
  {
    /*   Three species
    time
    0     a
    |     |
    1     Immigration
    |     |
    2  +--+--+
    |  |     |
    3  |     |      a
    |  |     |      |
    4  b     c      a
    */
    const double colonisation_time1{1.0};
    const double time_of_cladogenesis{2.0};
    const double colonisation_time2{3.0};
    species a = create_new_test_species(location::mainland);
    a.migrate_to_island(colonisation_time1);
    assert(a.get_times_of_colonization().size() == 1);
    a.go_extinct(time_of_cladogenesis, location::island);
    const species b = create_descendant(a, time_of_cladogenesis, location::island);
    const species c = create_descendant(a, time_of_cladogenesis, location::island);
    a.migrate_to_island(colonisation_time2);
    assert(a.get_times_of_colonization().size() == 2);
    const std::vector<species> pop{a,b,c};
    BOOST_CHECK(multiple_times_colonisation(pop));
  }
}

BOOST_AUTO_TEST_CASE(elly_conclude_status)
{
  {
    //Three species
    //    a
    //    |
    //    Immigration
    //    |
    // +--+--+
    // |     |
    // b     c
    const double colonisation_time1{1.0};
    const double time_of_cladogenesis{2.0};
    species a = create_new_test_species(location::mainland);
    a.migrate_to_island(colonisation_time1);
    a.go_extinct(time_of_cladogenesis, location::island);
    const species b = create_descendant(a, time_of_cladogenesis, location::island);
    const species c = create_descendant(a, time_of_cladogenesis, location::island);
    const std::vector<species> pop{a,b,c};
    BOOST_CHECK_EQUAL(conclude_status(clade(pop)), daic::species_status::endemic);
  }
  {
    //Three species
    //    a
    //    |
    //    |Immigration
    //    |
    //    a
    const double colonisation_time1{1.0};
    species a = create_new_test_species(location::mainland);
    a.migrate_to_island(colonisation_time1);
    const std::vector<species> pop{a};
    BOOST_CHECK_EQUAL(conclude_status(clade(pop)), daic::species_status::non_endemic);
  }
  {
    /*   Three species
    time
    0     a
    |     |
    1     Immigration
    |     |
    2  +--+--+
    |  |     |
    3  |     |      a
    |  |     |      |
    4  b     c      a
    */
    const double colonisation_time1{1.0};
    const double time_of_cladogenesis{2.0};
    const double colonisation_time2{3.0};
    species a = create_new_test_species(location::mainland);
    a.migrate_to_island(colonisation_time1);
    a.go_extinct(time_of_cladogenesis, location::island);
    const species b = create_descendant(a, time_of_cladogenesis, location::island);
    const species c = create_descendant(a, time_of_cladogenesis, location::island);
    a.migrate_to_island(colonisation_time2);
    const std::vector<species> pop{a,b,c};
    BOOST_CHECK_EQUAL(conclude_status(clade(pop)), daic::species_status::endemic_non_endemic);
  }
  {
    /* Species a and b from the same clade both immigrate to island from mainland TODO
         +------+
         a      b
         |      |
         Immigration
         |      |
         a      b
     */
    const species x = create_new_test_species(location::mainland);
    species a = create_descendant(x, 0.0, location::mainland);
    species b = create_descendant(x, 0.0 , location::mainland);
    a.migrate_to_island(1.0);
    b.migrate_to_island(2.0);
    std::vector<species> pop = {a,b};
    BOOST_CHECK_EQUAL(conclude_status(clade(pop)), daic::species_status::non_endemic);
  }
  {
    /* Species a and b from the same clade both immigrate to island from mainland and speciate
           +---------+
           a         b
           |         |
           Immigration
           |         |
        +--+--+   +--+--+
        |     |   |     |
        c     d   e     f
     */
    const species x = create_new_test_species(location::mainland);
    species a = create_descendant(x, 0.0, location::mainland);
    species b = create_descendant(x, 0.0, location::mainland);
    a.migrate_to_island(1.0);
    b.migrate_to_island(2.0);
    a.go_extinct(3.0,location::island);
    const species c = create_descendant(a, 3.0, location::island);
    const species d = create_descendant(a, 3.0, location::island);
    b.go_extinct(4.0, location::island);
    const species e = create_descendant(b, 4.0, location::island);
    const species f = create_descendant(b, 4.0, location::island);
    std::vector<species> pop = {a,b,c,d,e,f};
    BOOST_CHECK_EQUAL(conclude_status(clade(pop)), daic::species_status::endemic_non_endemic);

  }
}


BOOST_AUTO_TEST_CASE(elly_convert_to_daisie_input_with_multiple_colonizations)
{
  /*
   Time
   0                         Mainland:  a
   |                                    |
   1   Island:    a-   -   -   -   -   -|
   |              |                     |
   2           +--+--+                  |
   |           |     |                  |
   3        +--+--+  |                  |
   |        |     |  |                  |
   4        |     |  |       a-   -   - |
   |        |     |  |       |          |
   5        |     |  |    +--+--+       |
   |        |     |  |    |     |       |
   6        b     c  d    e     f       a
PRESENT

      a reimmigrates after already diversifying on the island
      first time of colonisation (1) should in this case be added to
      branching times, as well as the times of speciation for re-immigrated
      species a.
   */
  elly::parameters p = create_parameters_set4();

  simulation s(p);

  //Migration
  s.do_next_event(1.0, event::migration_to_island);
  s.do_next_event(1.0, event::clad_glob_on_island);
  s.do_next_event(1.0, event::clad_island_only);
  s.do_next_event(1.0, event::migration_to_island);
  s.do_next_event(1.0, event::clad_glob_on_island);
  BOOST_CHECK_EQUAL(count_clades(s.get_populations()), 1);

  const auto simulation_results = get_results(s);
  const daic::input i = convert_ideal(simulation_results);
  BOOST_REQUIRE_EQUAL(i.get().size(), 1);
  const daic::input_row row = i.get().back();
  const std::vector<double> branching_times = row.get_branching_times();
  BOOST_CHECK_EQUAL(row.get_n_missing_species(), 0);
  BOOST_CHECK_EQUAL(branching_times.size(), 4);
  BOOST_CHECK(std::count(branching_times.begin(), branching_times.end(), 1.0));
  BOOST_CHECK(std::count(branching_times.begin(), branching_times.end(), 2.0));
  BOOST_CHECK(std::count(branching_times.begin(), branching_times.end(), 3.0));
  BOOST_CHECK(std::count(branching_times.begin(), branching_times.end(), 5.0));
  BOOST_CHECK(conclude_status(clade(s.get_populations().get_species()))
              == daic::species_status::endemic_non_endemic);
}

BOOST_AUTO_TEST_CASE(elly_convert_to_daisie_input_with_multiple_colonizations_using_to_reality)
{
  // Same as picture of 'elly_convert_to_daisie_input_with_multiple_colonizations'
  elly::parameters p = create_parameters_set4();

  simulation s(p);

  //Migration
  s.do_next_event(1.0, event::migration_to_island);
  s.do_next_event(1.0, event::clad_glob_on_island);
  s.do_next_event(1.0, event::clad_island_only);
  s.do_next_event(1.0, event::migration_to_island);
  s.do_next_event(1.0, event::clad_glob_on_island);

  const auto simulation_results = get_results(s);
  BOOST_CHECK_NO_THROW(convert_reality(simulation_results));
}



BOOST_AUTO_TEST_CASE(elly_convert_ideal)
{
  const elly::parameters p = create_parameters_set2();
  simulation s(p);
  s.run();
  const auto simulation_results = get_results(s);
  const daic::input i = convert_ideal(simulation_results);
  BOOST_CHECK(!is_empty(i));
}

BOOST_AUTO_TEST_CASE(elly_convert_reality)
{
  //Must have results
  {
    const elly::parameters p = create_parameters_set2();
    simulation s(p);
    s.run();
    const auto simulation_results = get_results(s);
    const daic::input i = convert_reality(simulation_results);
    BOOST_CHECK(!is_empty(i));
  }
  {
    /*   Three species
    time
    0     a
    |     |
    1     Immigration
    |     |
    2  +--+--+
    |  |     |
    3  |     |
    |  |     |
    4  b     c
    */
    const elly::parameters p = create_parameters_set4();
    simulation s(p);
    elly::populations pop = s.get_populations();
    species a = pop.extract_random_species(location::mainland, s.get_rng());
    a.migrate_to_island(1.0);
    a.go_extinct(2.0, location::island);
    pop.add_species(a);
    const species b = create_descendant(a, 2.0, location::island);
    const species c = create_descendant(a, 2.0, location::island);
    pop.add_species(b);
    pop.add_species(c);
    BOOST_REQUIRE_EQUAL(count_colonists(pop.get_species()), 1);
    const auto simulation_results = get_results(pop);
    const daic::input i = convert_reality(simulation_results);
    BOOST_CHECK_EQUAL(static_cast<int>(i.get().size()) , 1);
    const daic::input_row row = i.get()[0];
    const std::vector<double> expected_brts = { 1.0, 2.0 };
    const std::vector<double> measured_brts = row.get_branching_times();
    BOOST_CHECK_EQUAL_COLLECTIONS(
      std::begin(measured_brts), std::end(measured_brts),
      std::begin(expected_brts), std::end(expected_brts)
    );

  }
}

//#define FIX_ISSUE_184_B
#ifdef FIX_ISSUE_184_B
BOOST_AUTO_TEST_CASE(elly_convert_reality_with_multiple_colonizations)
{
  const elly::parameters p = create_parameters_set3();
  simulation s(p);
  s.run();
  std::clog
    << "elly_convert_reality_with_multiple_colonizations" << '\n'
    << "------------------------------------------------" << '\n'
    << get_results(s) << '\n'
    << "------------------------------------------------" << '\n'
  ;
  const auto simulation_results = get_results(s);
  const daic::input i = convert_reality(simulation_results);

  BOOST_CHECK(!is_empty(i));
}
#endif // FIX_ISSUE_184_B


#pragma GCC diagnostic pop
