#include "elly_result.h"

#include "elly_location.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace elly;

BOOST_AUTO_TEST_CASE(elly_scenario_1)
{
  const species_id id = create_new_species_id();
  const species_id pid = create_new_species_id();
  const double t_birth_mainland{1.1};
  const double t_birth_island{-1.0};
  const double t_ext_mainland{7.8};
  const double t_ext_island{-1.0};
  const double t_colonization{5.1};
  const clade_id cid = create_new_clade_id();

  species s(id, pid, cid, t_birth_mainland, location::mainland);
  s.migrate_to_island(t_colonization);
  s.go_extinct(t_ext_mainland, location::mainland);

  const result r(s);
  std::stringstream measured;
  measured << r;

  const std::string expected{
    result_to_str(
      id,
      pid,
      t_birth_mainland,
      t_birth_island,
      t_ext_mainland,
      t_ext_island,
      t_colonization,
      cid
    )
  };

  BOOST_CHECK_EQUAL(measured.str(), expected);
}

BOOST_AUTO_TEST_CASE(elly_scenario_2)
{
  const species_id id = create_new_species_id();
  const species_id pid = create_new_species_id();
  const double t_birth_mainland{2.5};
  const double t_birth_island{-1.0};
  const double t_ext_mainland{-1.0};
  const double t_ext_island{8.5};
  const double t_colonization{6.2};
  const clade_id cid = create_new_clade_id();

  species s(id, pid, cid, t_birth_mainland, location::mainland);
  s.migrate_to_island(t_colonization);
  s.go_extinct(t_ext_island, location::island);

  const result r(s);
  std::stringstream measured;
  measured << r;
  const std::string expected{
    result_to_str(
      id,
      pid,
      t_birth_mainland,
      t_birth_island,
      t_ext_mainland,
      t_ext_island,
      t_colonization,
      cid
    )
  };

  BOOST_CHECK_EQUAL(measured.str() , expected);
}

BOOST_AUTO_TEST_CASE(elly_scenario_3)
{
  const species_id id = create_new_species_id();
  const species_id pid = create_new_species_id();
  const double t_birth_mainland{-1.0};
  const double t_birth_island{3.2};
  const double t_ext_mainland{-1.0};
  const double t_ext_island{-1.0};
  const double t_colonization{-1.0};

  const clade_id cid = create_new_clade_id();

  species s(id, pid, cid, t_birth_island, location::island);

  const result r(s);
  std::stringstream measured;
  measured << r;

  const std::string expected{
    result_to_str(
      id,
      pid,
      t_birth_mainland,
      t_birth_island,
      t_ext_mainland,
      t_ext_island,
      t_colonization,
      cid
    )
  };
  BOOST_CHECK_EQUAL(measured.str() , expected);
}

#pragma GCC diagnostic pop


