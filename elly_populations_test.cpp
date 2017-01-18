#include "elly_populations.h"

#include "elly_parameters.h"
#include "elly_location.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace elly;

BOOST_AUTO_TEST_CASE(elly_populations_construction)
{
  const auto params = create_parameters_set1();
  const int n{params.get_init_n_main_sps()};
  const populations pops(params);
  BOOST_CHECK_EQUAL(pops.count_species(location::both), 0);
  BOOST_CHECK_EQUAL(pops.count_species(location::island), 0);
  BOOST_CHECK_EQUAL(pops.count_species(location::island_only), 0);
  BOOST_CHECK_EQUAL(pops.count_species(location::mainland), n);
  BOOST_CHECK_EQUAL(pops.count_species(location::mainland_only), n);
  BOOST_CHECK_EQUAL(pops.count_extinct_species(), 0);
}

BOOST_AUTO_TEST_CASE(elly_count_clades)
{
  {
    const species a(create_new_species_id(), create_null_species_id(), create_new_clade_id(),
                    0.0, location::mainland);
    const species b(create_new_species_id(), create_null_species_id(), create_new_clade_id(),
                    0.0, location::mainland);
    const std::vector<species> pop = {a, b};
    const populations p(pop);
    BOOST_CHECK_EQUAL(count_clades(p), 2);
  }
  {
    const species a(create_new_species_id(), create_null_species_id(), create_new_clade_id(),
                    0.0, location::mainland);
    const species b(create_new_species_id(), create_null_species_id(), create_new_clade_id(),
                    0.0, location::mainland);
    const species c = create_descendant(a, 0.0, location::mainland);
    const std::vector<species> pop = {a, b, c};
    const populations p(pop);
    BOOST_CHECK_EQUAL(count_clades(p), 2);
  }
}

BOOST_AUTO_TEST_CASE(elly_populations_construction_less_clades_than_species)
{
  const parameters p = create_parameters_set2();
  const int c{p.get_init_n_main_cls()};
  const int n{p.get_init_n_main_sps()};
  const populations pop(p);
  BOOST_CHECK(count_clades(pop) == 8);
  BOOST_CHECK(count_clades(pop) < n);
  BOOST_CHECK(count_clades(pop) == c);
}

BOOST_AUTO_TEST_CASE(elly_create_test_populations_1)
{
  const populations pops = create_test_populations_1();
  BOOST_CHECK_EQUAL(pops.count_species(location::both), 1);
  BOOST_CHECK_EQUAL(pops.count_species(location::island), 2);
  BOOST_CHECK_EQUAL(pops.count_species(location::island_only), 1);
  BOOST_CHECK_EQUAL(pops.count_species(location::mainland), 2);
  BOOST_CHECK_EQUAL(pops.count_species(location::mainland_only), 1);
  BOOST_CHECK_EQUAL(pops.count_extinct_species(), 1);
}

BOOST_AUTO_TEST_CASE(elly_mainland_cladogenesis)
{
  {
    populations pops = create_test_populations_1();
    const double t{4.0};
    std::mt19937 rng(42);
    cladogenesis_mainland_only(pops, t, rng);
    BOOST_CHECK_EQUAL(pops.count_species(location::both), 1);
    BOOST_CHECK_EQUAL(pops.count_species(location::island), 2);
    BOOST_CHECK_EQUAL(pops.count_species(location::island_only), 1);
    BOOST_CHECK_EQUAL(pops.count_species(location::mainland), 2 - 1 + 2);
    BOOST_CHECK_EQUAL(pops.count_species(location::mainland_only), 1 - 1 + 2);
    BOOST_CHECK_EQUAL(pops.count_extinct_species(), 1 + 1);
  }
}


BOOST_AUTO_TEST_CASE(elly_mainland_extinction)
{
  {
    populations pops = create_test_populations_1();
    const double t{4.0};
    std::mt19937 rng(42);
    mainland_extinction(pops, t, rng);

    BOOST_CHECK_EQUAL(pops.count_species(location::both), 1);
    BOOST_CHECK_EQUAL(pops.count_species(location::island), 2);
    BOOST_CHECK_EQUAL(pops.count_species(location::island_only), 1);
    BOOST_CHECK_EQUAL(pops.count_species(location::mainland), 1);
    BOOST_CHECK_EQUAL(pops.count_species(location::mainland_only), 0);
    BOOST_CHECK_EQUAL(pops.count_extinct_species(), 2);
  }
}

BOOST_AUTO_TEST_CASE(elly_mainland_immigration)
{

  {
    populations pops = create_test_populations_1();
    const double t{4.0};
    std::mt19937 rng(42);
    mainland_immigration(pops, t, rng);

    BOOST_CHECK_EQUAL(pops.count_species(location::both), 2);
    BOOST_CHECK_EQUAL(pops.count_species(location::island), 3);
    BOOST_CHECK_EQUAL(pops.count_species(location::island_only), 1);
    BOOST_CHECK_EQUAL(pops.count_species(location::mainland), 2);
    BOOST_CHECK_EQUAL(pops.count_species(location::mainland_only), 0);
    BOOST_CHECK_EQUAL(pops.count_extinct_species(), 1);
  }
}

BOOST_AUTO_TEST_CASE(elly_island_extinction)
{

  {
    populations pops = create_test_populations_1();
    const double t{4.0};
    std::mt19937 rng(42);
    island_extinction(pops, t, rng);

    BOOST_CHECK_EQUAL(pops.count_species(location::both), 1);
    BOOST_CHECK_EQUAL(pops.count_species(location::island), 1);
    BOOST_CHECK_EQUAL(pops.count_species(location::island_only), 0);
    BOOST_CHECK_EQUAL(pops.count_species(location::mainland), 2);
    BOOST_CHECK_EQUAL(pops.count_species(location::mainland_only), 1);
    BOOST_CHECK_EQUAL(pops.count_extinct_species(), 2);
  }
}


BOOST_AUTO_TEST_CASE(elly_island_cladogenesis)
{

  {
    populations pops = create_test_populations_1();
    const double t{4.0};
    std::mt19937 rng(42);
    cladogenesis_island_only(pops, t, rng);

    BOOST_CHECK_EQUAL(pops.count_species(location::both), 1);
    BOOST_CHECK_EQUAL(pops.count_species(location::island), 3);
    BOOST_CHECK_EQUAL(pops.count_species(location::island_only), 2);
    BOOST_CHECK_EQUAL(pops.count_species(location::mainland), 2);
    BOOST_CHECK_EQUAL(pops.count_species(location::mainland_only), 1);
    BOOST_CHECK_EQUAL(pops.count_extinct_species(), 2);
  }
}

BOOST_AUTO_TEST_CASE(elly_both_extinction_island)
{

  {
    populations pops = create_test_populations_1();
    const double t{4.0};
    std::mt19937 rng(42);
    both_extinction_island(pops, t, rng);

    BOOST_CHECK_EQUAL(pops.count_species(location::both), 0);
    BOOST_CHECK_EQUAL(pops.count_species(location::island), 1);
    BOOST_CHECK_EQUAL(pops.count_species(location::island_only), 1);
    BOOST_CHECK_EQUAL(pops.count_species(location::mainland), 2);
    BOOST_CHECK_EQUAL(pops.count_species(location::mainland_only), 2);
    BOOST_CHECK_EQUAL(pops.count_extinct_species(), 1);
  }
}

BOOST_AUTO_TEST_CASE(elly_both_extinction_mainland)
{

  {
    populations pops = create_test_populations_1();
    const double t{4.0};
    std::mt19937 rng(42);
    both_extinction_mainland(pops, t, rng);

    BOOST_CHECK_EQUAL(pops.count_species(location::both), 0);
    BOOST_CHECK_EQUAL(pops.count_species(location::island), 2);
    BOOST_CHECK_EQUAL(pops.count_species(location::island_only), 2);
    BOOST_CHECK_EQUAL(pops.count_species(location::mainland), 1);
    BOOST_CHECK_EQUAL(pops.count_species(location::mainland_only), 1);
    BOOST_CHECK_EQUAL(pops.count_extinct_species(), 1);
  }
}

BOOST_AUTO_TEST_CASE(elly_both_anagenesis)
{

  {
    populations pops = create_test_populations_1();
    const double t{4.0};
    std::mt19937 rng(42);
    both_anagenesis(pops, t, rng);

    BOOST_CHECK_EQUAL(pops.count_species(location::both), 0);
    BOOST_CHECK_EQUAL(pops.count_species(location::island), 2);
    BOOST_CHECK_EQUAL(pops.count_species(location::island_only), 2);
    BOOST_CHECK_EQUAL(pops.count_species(location::mainland), 2);
    BOOST_CHECK_EQUAL(pops.count_species(location::mainland_only), 2);
    BOOST_CHECK_EQUAL(pops.count_extinct_species(), 1);
  }
}

BOOST_AUTO_TEST_CASE(elly_both_cladogenesis_island)
{
  {
    populations pops = create_test_populations_1();
    const double t{4.0};
    std::mt19937 rng(42);
    cladogenesis_global_on_island(pops, t, rng);
    BOOST_CHECK_EQUAL(pops.count_species(location::both), 0);
    BOOST_CHECK_EQUAL(pops.count_species(location::island), 3);
    BOOST_CHECK_EQUAL(pops.count_species(location::island_only), 3);
    BOOST_CHECK_EQUAL(pops.count_species(location::mainland), 2);
    BOOST_CHECK_EQUAL(pops.count_species(location::mainland_only), 2);
    BOOST_CHECK_EQUAL(pops.count_extinct_species(), 1);
  }
}

BOOST_AUTO_TEST_CASE(elly_both_cladogenesis_mainland)
{
  {
    populations pops = create_test_populations_1();
    const double t{4.0};
    std::mt19937 rng(42);
    cladogenesis_global_on_mainland(pops, t, rng);
    BOOST_CHECK_EQUAL(pops.count_species(location::both), 0);
    BOOST_CHECK_EQUAL(pops.count_species(location::island), 2);
    BOOST_CHECK_EQUAL(pops.count_species(location::island_only), 2);
    BOOST_CHECK_EQUAL(pops.count_species(location::mainland), 3);
    BOOST_CHECK_EQUAL(pops.count_species(location::mainland_only), 3);
    BOOST_CHECK_EQUAL(pops.count_extinct_species(), 1);
  }
}

BOOST_AUTO_TEST_CASE(elly_collect_species_with_clade_id)
{
  //Two clades: A has two species, B has one
  const clade_id id_a = create_new_clade_id();
  const clade_id id_b = create_new_clade_id();
  const clade_id id_c = create_new_clade_id();

  const species a(
    create_new_species_id(),
    create_null_species_id(),
    id_a,
    0.0,
    location::mainland
  );
  const species b(
    create_new_species_id(),
    create_null_species_id(),
    id_a,
    1.0,
    location::mainland
  );
  const species c(
    create_new_species_id(),
    create_null_species_id(),
    id_b,
    0.0,
    location::mainland
  );
  BOOST_CHECK_EQUAL(collect_species_with_clade_id(c, id_a).size(), 2);
  BOOST_CHECK_EQUAL(collect_species_with_clade_id(c, id_b).size(), 1);
  BOOST_CHECK_EQUAL(collect_species_with_clade_id(c, id_c).size(), 0);
}


#pragma GCC diagnostic pop

