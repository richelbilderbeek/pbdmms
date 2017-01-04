#include "elly_species.h"

#include "elly_location.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace elly;

BOOST_AUTO_TEST_CASE(elly_species_constructor)
{
  //Mainland species
  {
    const species_id pid = create_new_species_id();
    const species_id id = create_new_species_id();
    const clade_id cid = create_new_clade_id();
    const double time_of_birth{1.1};
    const location location_of_birth = location::mainland;
    const species s(id, pid, cid, time_of_birth, location_of_birth);
    BOOST_CHECK_EQUAL(s.get_species_id(), id);
    BOOST_CHECK_EQUAL(s.get_parent_id(), pid);
    BOOST_CHECK_EQUAL(s.get_clade_id(), cid);
    BOOST_CHECK_EQUAL(s.get_time_of_birth(), time_of_birth);
    BOOST_CHECK_EQUAL(s.get_location_of_birth(), location_of_birth);
  }
  //Island species
  {
    const species_id pid = create_new_species_id();
    const species_id id = create_new_species_id();
    const clade_id cid = create_new_clade_id();
    const double time_of_birth{2.2};
    const location location_of_birth = location::island;
    const species s(id, pid, cid, time_of_birth, location_of_birth);
    BOOST_CHECK_EQUAL(s.get_species_id(), id);
    BOOST_CHECK_EQUAL(s.get_parent_id(), pid);
    BOOST_CHECK_EQUAL(s.get_clade_id(), cid);
    BOOST_CHECK_EQUAL(s.get_time_of_birth(), time_of_birth);
    BOOST_CHECK_EQUAL(s.get_location_of_birth(), location_of_birth);
  }
}

BOOST_AUTO_TEST_CASE(elly_create_new_test_species)
{
  //Mainland species
  {
    const double time_of_birth{1.1};
    const location location_of_birth = location::mainland;
    const species s = create_new_test_species(time_of_birth, location_of_birth);
    BOOST_CHECK_EQUAL(s.get_location_of_birth(), location_of_birth);
    BOOST_CHECK_EQUAL(s.get_time_of_birth(), time_of_birth);
  }
  //Island species
  {
    const double time_of_birth{2.2};
    const location location_of_birth = location::island;
    const species s = create_new_test_species(time_of_birth, location_of_birth);
    BOOST_CHECK_EQUAL(s.get_location_of_birth(), location_of_birth);
    BOOST_CHECK_EQUAL(s.get_time_of_birth(), time_of_birth);
  }
}

BOOST_AUTO_TEST_CASE(elly_get_t_birth_mainland)
{
  //Mainland species
  {
    const double time_of_birth{1.1};
    const location location_of_birth = location::mainland;
    const species s = create_new_test_species(time_of_birth, location_of_birth);
    BOOST_CHECK_EQUAL(get_t_birth_mainland(s), time_of_birth);
  }
  //Island species
  {
    const double time_of_birth{2.2};
    const location location_of_birth = location::island;
    const species s = create_new_test_species(time_of_birth, location_of_birth);
    BOOST_CHECK_EQUAL(get_t_birth_mainland(s), -1.0);
  }
}

BOOST_AUTO_TEST_CASE(elly_get_t_birth_island)
{
  //Mainland species
  {
    const double time_of_birth{1.1};
    const location location_of_birth = location::mainland;
    const species s = create_new_test_species(time_of_birth, location_of_birth);
    BOOST_CHECK_EQUAL(get_t_birth_island(s), -1.0);
  }
  //Island species
  {
    const double time_of_birth{2.2};
    const location location_of_birth = location::island;
    const species s = create_new_test_species(time_of_birth, location_of_birth);
    BOOST_CHECK_EQUAL(get_t_birth_island(s), time_of_birth);
  }
}

BOOST_AUTO_TEST_CASE(elly_get_t_ext_mainland)
{
  //Mainland species, extant
  {
    const double time_of_birth{1.1};
    const location location_of_birth = location::mainland;
    const species s = create_new_test_species(time_of_birth, location_of_birth);
    BOOST_CHECK_EQUAL(get_t_ext_mainland(s), -1.0);
  }
  //Mainland species, extinct
  {
    const double time_of_birth{1.1};
    const double time_of_extinction{2.2};
    const location location_of_birth = location::mainland;
    species s = create_new_test_species(time_of_birth, location_of_birth);
    s.go_extinct(time_of_extinction, location::mainland);
    BOOST_CHECK_EQUAL(get_t_ext_mainland(s), time_of_extinction);
  }
  //Island species, extant
  {
    const double time_of_birth{2.2};
    const location location_of_birth = location::island;
    const species s = create_new_test_species(time_of_birth, location_of_birth);
    BOOST_CHECK_EQUAL(get_t_ext_mainland(s), -1.0);
  }
  //Island species, extinct
  {
    const double time_of_birth{2.2};
    const double time_of_extinction{3.3};
    const location location_of_birth = location::island;
    species s = create_new_test_species(time_of_birth, location_of_birth);
    s.go_extinct(time_of_extinction, location::island);
    BOOST_CHECK_EQUAL(get_t_ext_mainland(s), -1.0);
  }
}

BOOST_AUTO_TEST_CASE(elly_get_t_ext_island)
{
  //Mainland species, extant
  {
    const double time_of_birth{1.1};
    const location location_of_birth = location::mainland;
    const species s = create_new_test_species(time_of_birth, location_of_birth);
    BOOST_CHECK_EQUAL(get_t_ext_island(s), -1.0);
  }
  //Mainland species, extinct
  {
    const double time_of_birth{1.1};
    const double time_of_extinction{2.2};
    const location location_of_birth = location::mainland;
    species s = create_new_test_species(time_of_birth, location_of_birth);
    s.go_extinct(time_of_extinction, location::mainland);
    BOOST_CHECK_EQUAL(get_t_ext_island(s), -1.0);
  }
  //Island species, extant
  {
    const double time_of_birth{2.2};
    const location location_of_birth = location::island;
    const species s = create_new_test_species(time_of_birth, location_of_birth);
    BOOST_CHECK_EQUAL(get_t_ext_island(s), -1.0);
  }
  //Island species, extinct
  {
    const double time_of_birth{2.2};
    const double time_of_extinction{3.3};
    const location location_of_birth = location::island;
    species s = create_new_test_species(time_of_birth, location_of_birth);
    s.go_extinct(time_of_extinction, location::island);
    BOOST_CHECK_EQUAL(get_t_ext_island(s), time_of_extinction);
  }
}

BOOST_AUTO_TEST_CASE(elly_get_t_colonization)
{
  //Mainland species, does not migrate
  {
    const double time_of_birth{1.1};
    const location location_of_birth = location::mainland;
    const species s = create_new_test_species(time_of_birth, location_of_birth);
    BOOST_CHECK_EQUAL(get_t_colonization(s), -1.0);
  }
  //Mainland species, migrates once
  {
    const double time_of_birth{1.1};
    const double colonization_time{2.2};
    const location location_of_birth = location::mainland;
    species s = create_new_test_species(time_of_birth, location_of_birth);
    s.migrate_to_island(colonization_time);
    BOOST_CHECK_EQUAL(get_t_colonization(s), colonization_time);
  }
  //Mainland species, migrates twice, keep the latest value
  {
    const double time_of_birth{1.1};
    const double colonization_time_1{2.2};
    const double colonization_time_2{3.3};
    const location location_of_birth = location::mainland;
    species s = create_new_test_species(time_of_birth, location_of_birth);
    s.migrate_to_island(colonization_time_1);
    s.migrate_to_island(colonization_time_2);
    BOOST_CHECK_EQUAL(get_t_colonization(s), colonization_time_2);
  }
  //Island species, does not migrate
  {
    const double time_of_birth{2.2};
    const location location_of_birth = location::island;
    const species s = create_new_test_species(time_of_birth, location_of_birth);
    BOOST_CHECK_EQUAL(get_t_colonization(s), -1.0);
  }
  //Island species, cannot migrate to island
  {
    const double time_of_birth{2.2};
    const location location_of_birth = location::island;
    const double colonization_time{3.3};
    species s = create_new_test_species(time_of_birth, location_of_birth);
    BOOST_CHECK_THROW(
      s.migrate_to_island(colonization_time),
      std::logic_error
    );
  }
}

BOOST_AUTO_TEST_CASE(elly_is_extant)
{
  //Mainland species, no extinction
  {
    const species s = create_new_test_species(location::mainland);
    BOOST_CHECK(is_extant(s));
  }
  //Mainland species, extinction
  {
    species s = create_new_test_species(location::mainland);
    s.go_extinct(1.0, location::mainland);
    BOOST_CHECK(!is_extant(s));
  }
  //Island species, no extinction
  {
    const species s = create_new_test_species(location::island);
    BOOST_CHECK(is_extant(s));
  }
  //Mainland species, extinction
  {
    species s = create_new_test_species(location::island);
    s.go_extinct(1.0, location::island);
    BOOST_CHECK(!is_extant(s));
  }
  //Global species, no extinction
  {
    const species s = create_new_test_species(location::both);
    BOOST_CHECK(is_extant(s));
  }
  //Global species, extinction of island only
  {
    species s = create_new_test_species(location::both);
    s.go_extinct(1.0, location::island);
    BOOST_CHECK(is_extant(s));
  }
  //Global species, extinction of mainland only
  {
    species s = create_new_test_species(location::both);
    s.go_extinct(1.0, location::mainland);
    BOOST_CHECK(is_extant(s));
  }
  //Global species, global extinction
  {
    species s = create_new_test_species(location::both);
    s.go_extinct(1.0, location::island);
    s.go_extinct(2.0, location::mainland);
    BOOST_CHECK(!is_extant(s));
  }
}

BOOST_AUTO_TEST_CASE(elly_is_extinct)
{
  //Mainland species, no extinction
  {
    const species s = create_new_test_species(location::mainland);
    BOOST_CHECK(!is_extinct(s));
  }
  //Mainland species, extinction
  {
    species s = create_new_test_species(location::mainland);
    s.go_extinct(1.0, location::mainland);
    BOOST_CHECK(is_extinct(s));
  }
  //Island species, no extinction
  {
    const species s = create_new_test_species(location::island);
    BOOST_CHECK(!is_extinct(s));
  }
  //Mainland species, extinction
  {
    species s = create_new_test_species(location::island);
    s.go_extinct(1.0, location::island);
    BOOST_CHECK(is_extinct(s));
  }
  //Global species, no extinction
  {
    const species s = create_new_test_species(location::both);
    BOOST_CHECK(!is_extinct(s));
  }
  //Global species, extinction of island only
  {
    species s = create_new_test_species(location::both);
    s.go_extinct(1.0, location::island);
    BOOST_CHECK(!is_extinct(s));
  }
  //Global species, extinction of mainland only
  {
    species s = create_new_test_species(location::both);
    s.go_extinct(1.0, location::mainland);
    BOOST_CHECK(!is_extinct(s));
  }
  //Global species, global extinction
  {
    species s = create_new_test_species(location::both);
    s.go_extinct(1.0, location::island);
    s.go_extinct(2.0, location::mainland);
    BOOST_CHECK(is_extinct(s));
  }
}


BOOST_AUTO_TEST_CASE(elly_is_on_island)
{
  {
    const species s = create_new_test_species(location::mainland);
    BOOST_CHECK(!is_on_island(s));
  }
  {
    const species s = create_new_test_species(location::island);
    BOOST_CHECK(is_on_island(s));
  }
  {
    const species s = create_new_test_species(location::both);
    BOOST_CHECK(is_on_island(s));
  }
}

BOOST_AUTO_TEST_CASE(elly_is_on_island_only)
{
  {
    const species s = create_new_test_species(location::mainland);
    BOOST_CHECK(!is_on_island_only(s));
  }
  {
    const species s = create_new_test_species(location::island);
    BOOST_CHECK(is_on_island_only(s));
  }
  {
    const species s = create_new_test_species(location::both);
    BOOST_CHECK(!is_on_island_only(s));
  }
}

BOOST_AUTO_TEST_CASE(elly_is_on_mainland)
{
  {
    const species s = create_new_test_species(location::mainland);
    BOOST_CHECK(is_on_mainland(s));
  }
  {
    const species s = create_new_test_species(location::island);
    BOOST_CHECK(!is_on_mainland(s));
  }
  {
    const species s = create_new_test_species(location::both);
    BOOST_CHECK(is_on_mainland(s));
  }
}

BOOST_AUTO_TEST_CASE(elly_is_on_mainland_only)
{
  {
    const species s = create_new_test_species(location::mainland);
    BOOST_CHECK(is_on_mainland_only(s));
  }
  {
    const species s = create_new_test_species(location::island);
    BOOST_CHECK(!is_on_mainland_only(s));
  }
  {
    const species s = create_new_test_species(location::both);
    BOOST_CHECK(!is_on_mainland_only(s));
  }
}

BOOST_AUTO_TEST_CASE(elly_operator_equals_species)
{
  const species a = create_new_test_species(location::mainland);
  const species b(a);
  const species c = create_new_test_species(location::mainland);
  BOOST_CHECK(a == a);
  BOOST_CHECK(a == b);
  BOOST_CHECK(a != c);
  BOOST_CHECK(b == a);
  BOOST_CHECK(b == b);
  BOOST_CHECK(b != c);
  BOOST_CHECK(c != a);
  BOOST_CHECK(c != b);
  BOOST_CHECK(c == c);
}

BOOST_AUTO_TEST_CASE(elly_less_than_operator_species)
{
  const species a = create_new_test_species(1.0, location::mainland);
  const species b = create_new_test_species(2.0, location::mainland);
  const species c = create_new_test_species(3.0, location::mainland);
  BOOST_CHECK(a < b);
  BOOST_CHECK(b < c);
}

BOOST_AUTO_TEST_CASE(elly_species_scenario_1)
{
  /*
    +-------------------+ Migration +-------------------+
    |     Mainland      | at t = 5.1|      Island       |
    |                   |________ \ |                   |
    | Species born here |         / |                   |
    |  on t = 1.1       |           |                   |
    +-------------------+           +-------------------+
             |
             |                      Species has a time of birth mainland,
             \/                     time of extinction mainland,
      Extinction Mainland           and a time of immigration.
          at t = 7.8                The rest of parameters are left at -1.0

    */
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
  /*
  +-------------------+ Migration +-------------------+
  |     Mainland      | at t = 6.2|      Island       |
  |                   |________ \ |                   |
  | species born here |         / |                   |
  |  on t = 2.5       |           |                   |
  +-------------------+           +-------------------+
                                            |
                                            |
                                            \/
                                       Extinction on Island
                                         at t = 8.5

    This species has a time of birth mainland,
    time of migration and a time of extinction island.
    The rest of the parameters are left at -1.0
  */
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
  /*
  +-------------------+           +-------------------+
  |     Mainland      |           |      Island       |
  |                   |           |                   |
  |                   |           | species born here |
  |                   |           |    at t = 3.2     |
  +-------------------+           +-------------------+

  This species only has a time of birth island

  */
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
  /*                        First
    +-------------------+ Migration  +-------------------+
    |     Mainland      | at t = 5.1 |      Island       |
    |                   |___________\|                   |
    | species born here |           /|                   |
    |  on t = 3.2       |            |                   |
    |                   |___________\|                   |
    +-------------------+   Second  /+-------------------+
             |             Migration           |
             |             at t = 8.3          |
             \/                                \/
      Extinction Mainland                 Extinction Island
          at t = 12.8                       at t = 5.7

    This species had a time of birth mainland,
    time of migration and time of extinction island.
    At the second migration the time of migration has
    been updated at which point the time of extinction island
    resets to -1.0. time of extinction mainland is added afterwards.
  */
  const species_id pid = create_new_species_id();
  const species_id id = create_new_species_id();
  const clade_id cid = create_new_clade_id();
  const double time_of_birth{3.2};
  const double time_immigration1{4.2};
  const double time_immigration2{8.3};
  const double time_extinction_mainland{12.8};
  const double time_extinction_island{5.7};

  species s(id, pid, cid, time_of_birth, location::mainland);
  s.migrate_to_island(time_immigration1);
  s.go_extinct(time_extinction_island, location::island);
  s.migrate_to_island(time_immigration2);
  s.go_extinct(time_extinction_mainland, location::mainland);

  BOOST_CHECK(is_extant(s));
}

#pragma GCC diagnostic pop



