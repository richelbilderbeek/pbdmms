#include "elly_results.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace elly;
/*
BOOST_AUTO_TEST_CASE(elly_collect_branching_times_for_non_endemic)
{
  species a = create_new_test_species(location::mainland);
  const double t_migrate{1.0};
  a.migrate_to_island(t_migrate);
  const std::vector<species> population = { a  };
  const std::vector<double> v = collect_branching_times(population);
  BOOST_CHECK_EQUAL(v.size(), 1);
  BOOST_CHECK_CLOSE(v[0], t_migrate, 0.00001);
}

BOOST_AUTO_TEST_CASE(elly_collect_branching_times_single_endemic)
{
  species a = create_new_test_species(location::mainland);
  const double t_migrate{1.0};
  const double t_anagenesis{2.0};
  a.migrate_to_island(t_migrate);
  species b(create_new_species_id(), a.get_species_id(),
            a.get_clade_id(), t_anagenesis, location::island);
  std::vector<species> population = { a, b };
  const std::vector<double> v = collect_branching_times(population);
  BOOST_CHECK_EQUAL(v.size(), 1);
  BOOST_CHECK_CLOSE(v[0], b.get_time_of_birth(), 0.0001);
}

BOOST_AUTO_TEST_CASE(elly_collect_branching_times_two_branches)
{
  species a = create_new_test_species(location::mainland);
  const double t_migrate1{1.0};
  const double t_migrate2{2.0};
  const double t_diversify{1.5};
  a.migrate_to_island(t_migrate1);
  species b(create_new_species_id(), a.get_species_id(),
            a.get_clade_id(), t_diversify, location::island);
  species c(create_new_species_id(), a.get_species_id(),
            a.get_clade_id(), t_diversify, location::island);
  species d(create_new_species_id(), a.get_parent_id(), a.get_clade_id(), 0.0, location::mainland);
  c.migrate_to_island(t_migrate2);
  std::vector<species> population{a, b, c, d};
  const std::vector<double> v = collect_branching_times(population);
  BOOST_CHECK_EQUAL(v.size(), 2);
  BOOST_CHECK_CLOSE(v[0], b.get_time_of_birth(), 0.0001);
  BOOST_CHECK_CLOSE(v[1], c.get_time_of_birth(), 0.0001);
}
*/
#pragma GCC diagnostic pop
