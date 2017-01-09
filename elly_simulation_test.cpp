#include "elly_simulation.h"

#include "elly_parameters.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace elly;

BOOST_AUTO_TEST_CASE(elly_run_do_event_ana_fails_with_mainland_species_only)
{
  /*

 Mainland   Island
  +----+    +----+
  | 10 |    | 0  |
  +----+    +----+

  Anagenesis fails, as it only takes place on the island
  and there are no island species

  */
  const parameters p = create_parameters_set2();
  simulation s(p);
  BOOST_CHECK_THROW(
    s.do_next_event(1.0, event::ana), std::logic_error
  );
}

//ELLY_TODO

/*
  ana,
  /// A species that is on both location yields two new species. This happens on the island
  clad_glob_on_island,
  /// A species that is on both location yields two new species. This happens on the mainland
  clad_glob_on_main,
  /// A species that is on the island only yields two new species
  clad_island_only,
  /// A species that is on the mainland only yields two new species
  clad_main_only,
  /// A species that is on both location goes extinct on the island only
  ext_glob_on_island,
  /// A species that is on both location goes extinct on the mainland only
  ext_glob_on_main,
  /// A species that lives on the island only goes extinct
  ext_island_only,
  /// A species that lives on the mainland only goes extinct
  ext_main_only,
  /// A species can migrate from mainland to island
  migration_to_island
*/

#ifdef REALLY_NEED_CLADOGENESIS_OFFSPRING_WITH_PARENT_IDS_OF_DIFFERENT_SIGNS
BOOST_AUTO_TEST_CASE(elly_run_do_event_cladogenesis_gives_two_new_species_each_with_different_parent_id)
{
  /*

 Mainland   Island

 Initially, there is only one species on the mainland.
 Species A has an ID of, for example, 314

  +----+    +----+
  | A  |    | 0  |
  +----+    +----+

  Species A has cladogenesis at time = 1.0:
   * A goes extinct
   * B is born, has parent ID of  314
   * C is born, has parent ID of -314 (yes, minus 314)

  +----+    +----+
  | B  |    | 0  |
  | C  |    |    |
  +----+    +----+

  */
  const int init_n_mainland{1};
  const parameters p = create_parameters_set1(init_n_mainland);
  simulation s(p);
  assert(s.count_species(location::both) == 0);
  assert(s.count_species(location::island) == 0);
  assert(s.count_species(location::island_only) == 0);
  assert(s.count_species(location::mainland) == 1);
  assert(s.count_species(location::mainland_only) == 1);
  assert(!s.get_populations().get_species().empty());
  const auto a = s.get_populations().get_species().back();
  const species_id a_id = a.get_species_id();
  assert(a_id.get_id() > 0);

  s.do_next_event(1.0, event::clad_main_only);

  assert(s.count_species(location::both) == 0);
  assert(s.count_species(location::island) == 0);
  assert(s.count_species(location::island_only) == 0);
  assert(s.count_species(location::mainland) == 2);
  assert(s.count_species(location::mainland_only) == 2);
  assert(s.get_populations().get_species().size() == 3);

  const auto b = s.get_populations().get_species()[1];
  const auto c = s.get_populations().get_species()[2];

  BOOST_CHECK_EQUAL(b.get_parent_id().get_id(),  a_id.get_id());
  BOOST_CHECK_EQUAL(c.get_parent_id().get_id(), -a_id.get_id());
}
#endif // REALLY_NEED_CLADOGENESIS_OFFSPRING_WITH_PARENT_IDS_OF_DIFFERENT_SIGNS

BOOST_AUTO_TEST_CASE(elly_run_simulation)
{
  const parameters p = create_parameters_set1();
  simulation s(p);
  s.run();
}

#ifdef FIX_ISSUE_180
BOOST_AUTO_TEST_CASE(elly_replay_for_input_article_light)
{
  const auto s = replay_for_input_article_light();
  BOOST_CHECK(!s.get_measurements().empty());
}

BOOST_AUTO_TEST_CASE(elly_replay_for_input_article)
{
  const auto s = replay_for_input_article();
  BOOST_CHECK(!s.get_measurements().empty());
}
#endif // FIX_ISSUE_180

/* Only use for stress testing
BOOST_AUTO_TEST_CASE(elly_run_simulation_with_profiling_parameters)
{
  const parameters p = create_profiling_parameters();
  simulation s(p);
  BOOST_CHECK_NO_THROW(s.run());
}
*/

#pragma GCC diagnostic pop
