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
  const int n{params.get_init_n_mainland()};
  const populations pops(params);
  BOOST_CHECK_EQUAL(pops.count_species(location::both), 0);
  BOOST_CHECK_EQUAL(pops.count_species(location::island), 0);
  BOOST_CHECK_EQUAL(pops.count_species(location::island_only), 0);
  BOOST_CHECK_EQUAL(pops.count_species(location::mainland), n);
  BOOST_CHECK_EQUAL(pops.count_species(location::mainland_only), n);
  BOOST_CHECK_EQUAL(pops.count_extinct_species(), 0);
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
    mainland_cladogenesis(pops, t, rng);

    BOOST_CHECK_EQUAL(pops.count_species(location::both), 1);
    BOOST_CHECK_EQUAL(pops.count_species(location::island), 2);
    BOOST_CHECK_EQUAL(pops.count_species(location::island_only), 1);
    BOOST_CHECK_EQUAL(pops.count_species(location::mainland), 2 - 1 + 2);
    BOOST_CHECK_EQUAL(pops.count_species(location::mainland_only), 1 - 1 + 2);
    BOOST_CHECK_EQUAL(pops.count_extinct_species(), 1 + 1);
  }
}


BOOST_AUTO_TEST_CASE(TODO_elly_mainland_extinction)
{
  {
    populations pops = create_test_populations_1();
    const double t{4.0};
    std::mt19937 rng(42);
    mainland_extinction(pops, t, rng);

    BOOST_CHECK_EQUAL(pops.count_species(location::both), 1);
    BOOST_CHECK_EQUAL(pops.count_species(location::island), 2);
    BOOST_CHECK_EQUAL(pops.count_species(location::island_only), 1);
    BOOST_CHECK_EQUAL(pops.count_species(location::mainland), 2);
    BOOST_CHECK_EQUAL(pops.count_species(location::mainland_only), 1);
    BOOST_CHECK_EQUAL(pops.count_extinct_species(), 1);
  }
}

BOOST_AUTO_TEST_CASE(TODO_elly_mainland_immigration)
{
  {
    populations pops = create_test_populations_1();
    const double t{4.0};
    std::mt19937 rng(42);
    mainland_immigration(pops, t, rng);

    BOOST_CHECK_EQUAL(pops.count_species(location::both), 1);
    BOOST_CHECK_EQUAL(pops.count_species(location::island), 2);
    BOOST_CHECK_EQUAL(pops.count_species(location::island_only), 1);
    BOOST_CHECK_EQUAL(pops.count_species(location::mainland), 2);
    BOOST_CHECK_EQUAL(pops.count_species(location::mainland_only), 1);
    BOOST_CHECK_EQUAL(pops.count_extinct_species(), 1);
  }
}

BOOST_AUTO_TEST_CASE(TODO_elly_island_extinction)
{
  {
    populations pops = create_test_populations_1();
    const double t{4.0};
    std::mt19937 rng(42);
    island_extinction(pops, t, rng);

    BOOST_CHECK_EQUAL(pops.count_species(location::both), 1);
    BOOST_CHECK_EQUAL(pops.count_species(location::island), 2);
    BOOST_CHECK_EQUAL(pops.count_species(location::island_only), 1);
    BOOST_CHECK_EQUAL(pops.count_species(location::mainland), 2);
    BOOST_CHECK_EQUAL(pops.count_species(location::mainland_only), 1);
    BOOST_CHECK_EQUAL(pops.count_extinct_species(), 1);
  }
}


BOOST_AUTO_TEST_CASE(TODO_elly_island_cladogenesis)
{
  {
    populations pops = create_test_populations_1();
    const double t{4.0};
    std::mt19937 rng(42);
    island_cladogenesis(pops, t, rng);

    BOOST_CHECK_EQUAL(pops.count_species(location::both), 1);
    BOOST_CHECK_EQUAL(pops.count_species(location::island), 2);
    BOOST_CHECK_EQUAL(pops.count_species(location::island_only), 1);
    BOOST_CHECK_EQUAL(pops.count_species(location::mainland), 2);
    BOOST_CHECK_EQUAL(pops.count_species(location::mainland_only), 1);
    BOOST_CHECK_EQUAL(pops.count_extinct_species(), 1);
  }
}

BOOST_AUTO_TEST_CASE(TODO_elly_both_extinction_island)
{
  {
    populations pops = create_test_populations_1();
    const double t{4.0};
    std::mt19937 rng(42);
    both_extinction_island(pops, t, rng);

    BOOST_CHECK_EQUAL(pops.count_species(location::both), 1);
    BOOST_CHECK_EQUAL(pops.count_species(location::island), 2);
    BOOST_CHECK_EQUAL(pops.count_species(location::island_only), 1);
    BOOST_CHECK_EQUAL(pops.count_species(location::mainland), 2);
    BOOST_CHECK_EQUAL(pops.count_species(location::mainland_only), 1);
    BOOST_CHECK_EQUAL(pops.count_extinct_species(), 1);
  }
}

BOOST_AUTO_TEST_CASE(TODO_elly_both_extinction_mainland)
{
  {
    populations pops = create_test_populations_1();
    const double t{4.0};
    std::mt19937 rng(42);
    both_extinction_mainland(pops, t, rng);

    BOOST_CHECK_EQUAL(pops.count_species(location::both), 1);
    BOOST_CHECK_EQUAL(pops.count_species(location::island), 2);
    BOOST_CHECK_EQUAL(pops.count_species(location::island_only), 1);
    BOOST_CHECK_EQUAL(pops.count_species(location::mainland), 2);
    BOOST_CHECK_EQUAL(pops.count_species(location::mainland_only), 1);
    BOOST_CHECK_EQUAL(pops.count_extinct_species(), 1);
  }
}

BOOST_AUTO_TEST_CASE(TODO_elly_both_anagenesis)
{
  {
    populations pops = create_test_populations_1();
    const double t{4.0};
    std::mt19937 rng(42);
    both_anagenesis(pops, t, rng);

    BOOST_CHECK_EQUAL(pops.count_species(location::both), 1);
    BOOST_CHECK_EQUAL(pops.count_species(location::island), 2);
    BOOST_CHECK_EQUAL(pops.count_species(location::island_only), 1);
    BOOST_CHECK_EQUAL(pops.count_species(location::mainland), 2);
    BOOST_CHECK_EQUAL(pops.count_species(location::mainland_only), 1);
    BOOST_CHECK_EQUAL(pops.count_extinct_species(), 1);
  }
}

BOOST_AUTO_TEST_CASE(TODO_elly_both_cladogenesis_island)
{
  {
    populations pops = create_test_populations_1();
    const double t{4.0};
    std::mt19937 rng(42);
    both_cladogenesis_island(pops, t, rng);

    BOOST_CHECK_EQUAL(pops.count_species(location::both), 1);
    BOOST_CHECK_EQUAL(pops.count_species(location::island), 2);
    BOOST_CHECK_EQUAL(pops.count_species(location::island_only), 1);
    BOOST_CHECK_EQUAL(pops.count_species(location::mainland), 2);
    BOOST_CHECK_EQUAL(pops.count_species(location::mainland_only), 1);
    BOOST_CHECK_EQUAL(pops.count_extinct_species(), 1);
  }
}

BOOST_AUTO_TEST_CASE(TODO_elly_both_cladogenesis_mainland)
{
  {
    populations pops = create_test_populations_1();
    const double t{4.0};
    std::mt19937 rng(42);
    both_cladogenesis_mainland(pops, t, rng);

    BOOST_CHECK_EQUAL(pops.count_species(location::both), 1);
    BOOST_CHECK_EQUAL(pops.count_species(location::island), 2);
    BOOST_CHECK_EQUAL(pops.count_species(location::island_only), 1);
    BOOST_CHECK_EQUAL(pops.count_species(location::mainland), 2);
    BOOST_CHECK_EQUAL(pops.count_species(location::mainland_only), 1);
    BOOST_CHECK_EQUAL(pops.count_extinct_species(), 1);
  }
}

#pragma GCC diagnostic pop

