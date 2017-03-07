#include "sado_output.h"
#include "sado_population.h"
#include "sado_parameters.h"
#include "sado_results.h"
#include "sado_simulation.h"
#include <iostream>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace sado;

BOOST_AUTO_TEST_CASE(sado_zero_indivs_no_species)
{
  const population pop = create_test_population_0();
  assert(pop.empty());
  parameters par = create_article_parameters();
  results r(par);
  copy_indivs_to_species(pop, 0, r, par);

  BOOST_CHECK_EQUAL(static_cast<int>(r.get_species().size()), 0);
}

BOOST_AUTO_TEST_CASE(sado_one_indiv_one_species)
{
  const population pop = create_test_population_1();
  parameters par = create_article_parameters();
  results r(par);

  copy_indivs_to_species(pop, 0, r, par);

  BOOST_CHECK_EQUAL(static_cast<int>(r.get_species().size()), 1);
}

#ifdef FIX_ISSUE_249
BOOST_AUTO_TEST_CASE(sado_two_indivs_one_species)
{
  const population pop = create_test_population_2();

  parameters par = create_article_parameters();
  results r(par);

  copy_indivs_to_species(pop, 0, r, par);

  BOOST_CHECK_EQUAL(r.get_species().size(), 1);
}


BOOST_AUTO_TEST_CASE(sado_two_indivs_two_species)
{
  const population pop = create_test_population_3();

  parameters par = create_article_parameters();
  results r(par);

  copy_indivs_to_species(pop, 0, r, par);

  BOOST_CHECK_EQUAL(r.get_species().size(), 2);
}
#endif // FIX_ISSUE_249


#pragma GCC diagnostic pop


