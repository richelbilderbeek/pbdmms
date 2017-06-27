#include "sado_likelihood.h"

#include <cassert>

#include "sado_newick.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace sado;

BOOST_AUTO_TEST_CASE(sado_calc_max_likelihood)
{
  /*
  library(ape)
  library(PBD)

  p <- tryCatch(
    read.tree(text = "(:2,(:1,:1):1):1;"),
    error = function(e) {}
  )
  c <- ifelse(class(p) == "phylo", "Y", "N")
  my_file <- file("is_newick.txt")
  writeLines(c, my_file)
  close(my_file)


  ape::plot.phylo(p, root.edge = TRUE)

  ml <- pbd_ML(
    brts = ape::branching.times(p), # compared to crown
    initparsopt = c(0.2,0.01,0.3),
    exteq = 1 # mu_1 == mu_2
  )

  ml
  */

  /* OUTPUT

  You are optimizing b mu_1 lambda_1
  You are fixing nothing
  Extinction rate of incipient species is exactly the same as for good species.
  The likelihood for the initial parameter values is -3.134288
  Optimizing the likelihood - this may take a while.

   Maximum likelihood parameter estimates: b: 0.754740, mu_1: 0.841467, lambda_1: 0.779326, mu_2: 0.841467
   Maximum loglikelihood: -2.393492
   The expected duration of speciation for these parameters is: 0.655009
   The median duration of speciation for these parameters is: 0.482826

            b      mu_1  lambda_1      mu_2    loglik df conv
  1 0.7547398 0.8414671 0.7793264 0.8414671 -2.393492  3    0

  */

  const std::string newick{"(:2,(:1,:1):1):1;"};
  assert(is_newick(newick));
  const likelihood p = calc_max_likelihood(newick);
  BOOST_CHECK_CLOSE(p.get_b()       , 0.7547398, 0.0001);
  BOOST_CHECK_CLOSE(p.get_mu_1()    , 0.8414671, 0.0001);
  BOOST_CHECK_CLOSE(p.get_lambda_1(), 0.7793264, 0.0001);
  BOOST_CHECK_CLOSE(p.get_mu_2()    , 0.8414671, 0.0001);
  BOOST_CHECK_CLOSE(p.get_loglik()  , -2.393492, 0.0001);
  BOOST_CHECK_EQUAL(p.get_df()  , 3);
  BOOST_CHECK_EQUAL(p.get_conv(), 0);
}


#pragma GCC diagnostic pop



