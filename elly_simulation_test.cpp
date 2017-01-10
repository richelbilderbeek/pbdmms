#include "elly_simulation.h"

#include "elly_parameters.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace elly;

BOOST_AUTO_TEST_CASE(elly_simulation_extract_random_species)
{
  const parameters p = create_parameters_set2();
  assert(p.get_init_n_main_sps() >= 1);
  simulation s(p);
  BOOST_CHECK_EQUAL(s.count_species(location::mainland), p.get_init_n_main_sps());
  BOOST_CHECK_EQUAL(s.count_species(location::island), 0);

  //Extracting a species should reduce the number of species
  {
    const int n_before{s.count_species(location::mainland)};
    s.extract_random_species(location::mainland);
    const int n_after{s.count_species(location::mainland)};
    BOOST_CHECK_EQUAL(n_before - 1, n_after);
  }
  //Cannot extract absent species
  {
    BOOST_CHECK_THROW(
      s.extract_random_species(location::island), std::logic_error
    );
  }
}

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

BOOST_AUTO_TEST_CASE(elly_anagenesis_only_global_species)
{
  const parameters p = create_parameters_set2();
  simulation s(p);
  std::mt19937 rng(42);
  s.do_next_event(1.0, event::migration_to_island);
  s.do_next_event(2.0, event::clad_glob_on_island);
  const auto endemic = s.get_populations().get_random_species(location::island_only, rng);
  const auto mainlander = s.get_populations().get_random_species(location::mainland_only, rng);
  BOOST_CHECK_THROW(
    s.do_next_event(1.0, event::ana, mainlander), std::logic_error
  );
  BOOST_CHECK_THROW(
        s.do_next_event(1.0, event::ana, endemic), std::logic_error);
}
BOOST_AUTO_TEST_CASE(elly_clad_glob_on_island_only_global_species)
{
  const parameters p = create_parameters_set2();
  simulation s(p);
  std::mt19937 rng(42);
  s.do_next_event(1.0, event::migration_to_island);
  s.do_next_event(2.0, event::clad_glob_on_island);
  const auto endemic = s.get_populations().get_random_species(location::island_only, rng);
  const auto mainlander = s.get_populations().get_random_species(location::mainland_only, rng);
  BOOST_CHECK_THROW(
        s.do_next_event(1.0, event::clad_glob_on_island, endemic), std::logic_error);
  BOOST_CHECK_THROW(
        s.do_next_event(1.0, event::clad_glob_on_island, mainlander), std::logic_error);
}

BOOST_AUTO_TEST_CASE(elly_clad_glob_on_main_only_global_species)
{
  const parameters p = create_parameters_set2();
  simulation s(p);
  std::mt19937 rng(42);
  s.do_next_event(1.0, event::migration_to_island);
  s.do_next_event(2.0, event::clad_glob_on_island);
  const auto endemic = s.get_populations().get_random_species(location::island_only, rng);
  const auto mainlander = s.get_populations().get_random_species(location::mainland_only, rng);
  BOOST_CHECK_THROW(
        s.do_next_event(1.0, event::clad_glob_on_main, endemic), std::logic_error);
  BOOST_CHECK_THROW(
        s.do_next_event(1.0, event::clad_glob_on_main, mainlander), std::logic_error);
}

BOOST_AUTO_TEST_CASE(elly_clad_island_only_no_mainland_species)
{
  const parameters p = create_parameters_set2();
  simulation s(p);
  std::mt19937 rng(42);
  s.do_next_event(1.0, event::migration_to_island);
  s.do_next_event(2.0, event::clad_glob_on_island);
  s.do_next_event(3.0, event::migration_to_island);
  const auto global_species = s.get_populations().get_random_species(location::both, rng);
  const auto mainlander = s.get_populations().get_random_species(location::mainland_only, rng);
  BOOST_CHECK_THROW(
        s.do_next_event(1.0, event::clad_island_only, global_species), std::logic_error);
  BOOST_CHECK_THROW(
        s.do_next_event(1.0, event::clad_island_only, mainlander), std::logic_error);
}

BOOST_AUTO_TEST_CASE(elly_clad_main_only_no_island_species)
{
  const parameters p = create_parameters_set2();
  simulation s(p);
  std::mt19937 rng(42);
  s.do_next_event(1.0, event::migration_to_island);
  s.do_next_event(2.0, event::clad_glob_on_island);
  s.do_next_event(3.0, event::migration_to_island);
  const auto global_species = s.get_populations().get_random_species(location::both, rng);
  const auto endemic = s.get_populations().get_random_species(location::island_only, rng);
  BOOST_CHECK_THROW(
        s.do_next_event(1.0, event::clad_main_only, global_species), std::logic_error);
  BOOST_CHECK_THROW(
        s.do_next_event(1.0, event::clad_main_only, endemic), std::logic_error);
}

BOOST_AUTO_TEST_CASE(elly_ext_glob_on_island_only_global_species)
{
  const parameters p = create_parameters_set2();
  simulation s(p);
  std::mt19937 rng(42);
  s.do_next_event(1.0, event::migration_to_island);
  s.do_next_event(2.0, event::clad_glob_on_island);
  const auto mainlander = s.get_populations().get_random_species(location::mainland_only, rng);
  const auto endemic = s.get_populations().get_random_species(location::island_only, rng);
  BOOST_CHECK_THROW(
        s.do_next_event(1.0, event::ext_glob_on_island, mainlander), std::logic_error);
  BOOST_CHECK_THROW(
        s.do_next_event(1.0, event::ext_glob_on_island, endemic), std::logic_error);
}

BOOST_AUTO_TEST_CASE(elly_ext_glob_main_only_global_species)
{
  const parameters p = create_parameters_set2();
  simulation s(p);
  std::mt19937 rng(42);
  s.do_next_event(1.0, event::migration_to_island);
  s.do_next_event(2.0, event::clad_glob_on_island);
  const auto mainlander = s.get_random_species(location::mainland_only, rng);
  const auto endemic = s.get_random_species(location::island_only, rng);
  BOOST_CHECK_THROW(
        s.do_next_event(1.0, event::ext_glob_on_main, mainlander), std::logic_error);
  BOOST_CHECK_THROW(
        s.do_next_event(1.0, event::ext_glob_on_main, endemic), std::logic_error);
}

BOOST_AUTO_TEST_CASE(elly_ext_island_only_no_mainlanders)
{
  const parameters p = create_parameters_set2();
  simulation s(p);
  std::mt19937 rng(42);
  s.do_next_event(1.0, event::migration_to_island);
  s.do_next_event(2.0, event::clad_glob_on_island);
  s.do_next_event(3.0, event::migration_to_island);
  const auto global_species = s.get_random_species(location::both, rng);
  const auto mainlander = s.get_random_species(location::mainland_only, rng);
  BOOST_CHECK_THROW(
        s.do_next_event(1.0, event::ext_island_only, global_species), std::logic_error);
  BOOST_CHECK_THROW(
        s.do_next_event(1.0, event::ext_island_only, mainlander), std::logic_error);
}

BOOST_AUTO_TEST_CASE(elly_ext_main_only_no_islanders)
{
  const parameters p = create_parameters_set2();
  simulation s(p);
  std::mt19937 rng(42);
  s.do_next_event(1.0, event::migration_to_island);
  s.do_next_event(2.0, event::clad_glob_on_island);
  s.do_next_event(3.0, event::migration_to_island);
  const auto global_species = s.get_populations().get_random_species(location::both, rng);
  const auto endemic = s.get_populations().get_random_species(location::island_only, rng);
  BOOST_CHECK_THROW(
        s.do_next_event(1.0, event::ext_main_only, global_species), std::logic_error);
  BOOST_CHECK_THROW(
        s.do_next_event(1.0, event::ext_main_only, endemic), std::logic_error);
}

BOOST_AUTO_TEST_CASE(elly_migration_to_island_results_in_a_global_species)
{
  const parameters p = create_parameters_set2();
  simulation s(p);
  std::mt19937 rng(42);
  s.do_next_event(1.0, event::migration_to_island);

  const auto global_species = s.get_random_species(location::both, rng);
  BOOST_CHECK(is_on_both(global_species));
  BOOST_CHECK( is_on(global_species, location::both));
  BOOST_CHECK( is_on(global_species, location::island));
  BOOST_CHECK(!is_on(global_species, location::island_only));
  BOOST_CHECK( is_on(global_species, location::mainland));
  BOOST_CHECK(!is_on(global_species, location::mainland_only));
}

BOOST_AUTO_TEST_CASE(elly_migration_to_island_no_islanders)
{
  const parameters p = create_parameters_set2();
  simulation s(p);
  std::mt19937 rng(42);
  s.do_next_event(1.0, event::migration_to_island);
  s.do_next_event(2.0, event::clad_glob_on_island);
  s.do_next_event(3.0, event::migration_to_island);

  const auto global_species = s.get_populations().get_random_species(location::both, rng);
  const auto endemic = s.get_populations().get_random_species(location::island_only, rng);
  BOOST_CHECK_NO_THROW(
        s.do_next_event(4.0, event::migration_to_island, global_species));
  BOOST_CHECK_THROW(
        s.do_next_event(4.0, event::migration_to_island, endemic), std::logic_error);
}

BOOST_AUTO_TEST_CASE(elly_run_simulation)
{
  const parameters p = create_parameters_set1();
  simulation s(p);
  s.run();
}

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

#pragma GCC diagnostic pop
