#include "sado_individual.h"

#include "sado_parameters.h"

#include <fstream>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace sado;

BOOST_AUTO_TEST_CASE(sado_indiv_have_different_ids)
{
  const individual a;
  const individual b;
  BOOST_CHECK(a.get_id() != b.get_id());
}

BOOST_AUTO_TEST_CASE(sado_indiv_knows_its_parenst)
{
  const individual father;
  const individual mother;
  const parameters p = create_golden_standard_parameters();

  const individual kid = create_offspring(mother, father, p);
  BOOST_CHECK(kid.get_mother_id() == mother.get_id());
  BOOST_CHECK(kid.get_father_id() == father.get_id());
}

#pragma GCC diagnostic pop



