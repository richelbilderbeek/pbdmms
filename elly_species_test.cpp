#include "elly_species.h"

#include "elly_location.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace elly;

BOOST_AUTO_TEST_CASE(elly_species_scenario_1)
{
  const species_id pid = create_new_species_id();
  const species_id id = create_new_species_id();
  const clade_id cid = create_new_clade_id();
  const double time_of_birth{1.1};
  const double time_migrate_to_island{5.1};
  const double time_extinction_mainland{7.8};

  species s(id, pid, cid, time_of_birth, location::mainland);
  s.migrate_to_island(time_migrate_to_island);
  s.go_extinct(time_extinction_mainland, location::mainland);

  BOOST_CHECK_EQUAL(get_t_birth_mainland(s), time_of_birth);
  BOOST_CHECK_EQUAL(get_t_birth_island(s), -1.0);
  BOOST_CHECK_EQUAL(get_t_ext_mainland(s), time_extinction_mainland);
  BOOST_CHECK_EQUAL(get_t_ext_island(s), -1.0);
  BOOST_CHECK_EQUAL(get_t_colonization(s), time_migrate_to_island);
}

BOOST_AUTO_TEST_CASE(elly_species_scenario_2)
{
  const species_id pid = create_new_species_id();
  const species_id id = create_new_species_id();
  const clade_id cid = create_new_clade_id();
  const double time_of_birth{2.5};
  const double time_migrate_to_island{6.2};
  const double time_extinction_island{8.5};

  species s(id, pid, cid, time_of_birth, location::mainland);
  s.migrate_to_island(time_migrate_to_island);
  s.go_extinct(time_extinction_island, location::island);

  BOOST_CHECK_EQUAL(get_t_birth_mainland(s), time_of_birth);
  BOOST_CHECK_EQUAL(get_t_birth_island(s), -1.0);
  BOOST_CHECK_EQUAL(get_t_ext_mainland(s), -1.0);
  BOOST_CHECK_EQUAL(get_t_ext_island(s), time_extinction_island);
  BOOST_CHECK_EQUAL(get_t_colonization(s), time_migrate_to_island);
}

BOOST_AUTO_TEST_CASE(elly_species_scenario_3)
{
  const species_id pid = create_new_species_id();
  const species_id id = create_new_species_id();
  const clade_id cid = create_new_clade_id();
  const double time_of_birth{3.2};

  const species s(id, pid, cid, time_of_birth, location::island);

  BOOST_CHECK_EQUAL(get_t_birth_mainland(s), -1.0);
  BOOST_CHECK_EQUAL(get_t_birth_island(s), time_of_birth);
  BOOST_CHECK_EQUAL(get_t_ext_mainland(s), -1.0);
  BOOST_CHECK_EQUAL(get_t_ext_island(s), -1.0);
  BOOST_CHECK_EQUAL(get_t_colonization(s), -1.0);

}

BOOST_AUTO_TEST_CASE(elly_species_scenario_4)
{
  const species_id pid = create_new_species_id();
  const species_id id = create_new_species_id();
  const clade_id cid = create_new_clade_id();
  const double time_of_birth{3.2};
  const double time_immigration1{4.2};
  const double time_immigration2{8.3};
  const double time_extinction_mainland{12.8};
  const double time_extinction_island{5.7};

  const species s(id, pid, cid, time_of_birth, location::mainland);
  s.migrate_to_island(time_immigration1);
  s.go_extinct(time_extinction_island, location::island);
  s.migrate_to_island(time_immigration2);
  s.go_extinct(time_extinction_mainland);

  BOOST_CHECK(is_extant(s));
}

#pragma GCC diagnostic pop



