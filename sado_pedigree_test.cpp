#include "sado_pedigree.h"
#include "sado_individual.h"


#include <fstream>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace sado;

BOOST_AUTO_TEST_CASE(sado_pedigree_default_construction)
{
  pedigree p;
  BOOST_CHECK(p.empty());
}

BOOST_AUTO_TEST_CASE(sado_pedigree_add_invid)
{
  pedigree p;
  p.add(indiv());
  BOOST_CHECK(!p.empty());
}

BOOST_AUTO_TEST_CASE(sado_pedigree_comparison_operators)
{
  pedigree p;
  p.add(indiv());
  pedigree q;
  BOOST_CHECK(p == p);
  BOOST_CHECK(q == q);
  BOOST_CHECK(p != q);
}

BOOST_AUTO_TEST_CASE(sado_pedigree_are_indivs_related)
{
  pedigree p;
  indiv a;
  indiv b;
  p.add(a);
  p.add(b);
  BOOST_CHECK(!p.are_related(a, b));
}

#pragma GCC diagnostic pop

