#include <iostream>
#include "lyke_individual.h"
#include <cassert>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

#define BOOST_TEST_DYN_LINK // Defines a main function

#define BOOST_TEST_MODULE test_lyke_module
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(lyke_birth_individual_gives_mothers_or_father_genes)
{
  const Individual mother;
  const Individual father;
  //Must use & because of request for pointers as arguments
  const Individual offspring(mother, father);

  BOOST_CHECK(offspring.getX() == mother.getX() || offspring.getX() == father.getX());
}


#pragma GCC diagnostic pop
