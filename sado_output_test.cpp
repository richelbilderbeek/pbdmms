#include "sado_output.h"
#include "sado_population.h"
#include "sado_parameters.h"
#include "sado_results.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace sado;

BOOST_AUTO_TEST_CASE(sado_zero_indivs_no_species)
{
  population pop;
  assert(pop.empty());
  parameters par = create_article_parameters();
  results r(par);
  copy_indivs_to_species(pop, 0, r, par);

  BOOST_CHECK_EQUAL(static_cast<int>(r.get_species().size()), 0);
}

BOOST_AUTO_TEST_CASE(sado_one_indiv_one_species)
{
  population pop;
  indiv i;
  pop.add_indiv(i);
  parameters par = create_article_parameters();
  results r(par);

  copy_indivs_to_species(pop, 0, r, par);

  BOOST_CHECK_EQUAL(static_cast<int>(r.get_species().size()), 1);
}



#pragma GCC diagnostic pop


