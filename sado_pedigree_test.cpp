#include "sado_pedigree.h"

#ifdef REALLY_NEED_PEDIGREE

#include "sado_individual.h"


#include <fstream>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

#include "sado_parameters.h"

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


  /* a        b
   * |\____  /|
   *  \    \/ |
   *   \   /\ |
   *    \ /  \|
   *   kid1  kid2
   *     \    /
   *      \  /
   *       \/
   *      kid3
   *
   * a and b are not related
   * kids are related to a and b
   * kid1 is not related to kid2
   */

  pedigree p;
  indiv a;
  indiv b;
  p.add(a);
  p.add(b);
  BOOST_CHECK(!p.are_related(a, b));

  const auto par = create_article_parameters();
  indiv kid1 = create_offspring(a, b, par);
  p.add(kid1);
  BOOST_CHECK(p.are_related(a,kid1));
  BOOST_CHECK(p.are_related(b,kid1));

  indiv kid2 = create_offspring(a,b,par);
  p.add(kid2);
  BOOST_CHECK(p.are_related(a,kid2));
  BOOST_CHECK(p.are_related(b,kid2));
  BOOST_CHECK(!p.are_related(kid2,kid1));

  indiv kid3 = create_offspring(kid1, kid2, par);
  p.add(kid3);
  BOOST_CHECK(p.are_related(kid3, a));

}

#pragma GCC diagnostic pop


#endif // REALLY_NEED_PEDIGREE
