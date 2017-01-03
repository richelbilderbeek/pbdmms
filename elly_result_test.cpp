#include "elly_result.h"

#include "elly_location.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace elly;

BOOST_AUTO_TEST_CASE(elly_scenario_1)
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

  const result r(s);
  std::stringstream measured;
  measured << r;

  std::stringstream expected;
  expected << id << ',' << pid << ',' << time_of_birth << ',' << -1.0 << ','
    << time_extinction_mainland << ',' << -1.0 << ',' << time_migrate_to_island << ',' << cid;
  ;

  BOOST_CHECK_EQUAL(measured.str(), expected.str())    ;
}

BOOST_AUTO_TEST_CASE(elly_scenario_2)
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

  const result r(s);
  std::stringstream measured;
  measured << r;

  std::stringstream expected;
  expected << id << ',' << pid << ',' << time_of_birth << ',' << -1.0 << ',' << -1.0 << ',' << -1.0
           << ',' << time_extinction_island << ',' << time_migrate_to_island << ',' << cid
  ;

  BOOST_CHECK_EQUAL(measured.str() , expected.str())    ;
}

BOOST_AUTO_TEST_CASE(elly_scenario_3)
{
  const species_id pid = create_new_species_id();
  const species_id id = create_new_species_id();
  const clade_id cid = create_new_clade_id();
  const double time_of_birth{3.2};

  species s(id, pid, cid, time_of_birth, location::island);

  const result r(s);
  std::stringstream measured;
  measured << r;

  std::stringstream expected;
  expected << id << ',' << pid << ',' << -1.0 << ',' << time_of_birth << ',' << -1.0 << ',' << -1.0 <<  ','
              << -1.0 << ',' << -1.0 << ',' << cid
  ;

  BOOST_CHECK_EQUAL(measured.str() , expected.str())     ;
}

#pragma GCC diagnostic pop


