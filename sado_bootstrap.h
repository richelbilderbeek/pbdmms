#ifndef SADO_BOOTSTRAP_H
#define SADO_BOOTSTRAP_H

#include <iosfwd>
#include <vector>

#include "sado_likelihood.h"

namespace sado {

///Do a bootstrap analysis, by running simulations with the
///maximum likelihood values and obtaining the max likelihoods of those
///runs.
class bootstrap
{
public:
  bootstrap(
    const std::string& newick,
    const likelihood& max_likelihood,
    const int n,
    const int rng_seed
  );
private:
  std::vector<likelihood> m_likelihoods;

  friend std::ostream& operator<<(std::ostream& os, const bootstrap& b) noexcept;
};

///Do a bootstrap analysis, by running simulations with the
///maximum likelihood values and obtaining the max likelihoods of those
///runs.
/// @returns n maximum likelihoods
std::vector<likelihood> do_bootstrap(
  const std::string& newick,
  const likelihood& max_likelihood,
  const int n,
  const int rng_seed
);

std::vector<sado::likelihood> likelihoods_from_file(const std::string& filename);

/*
          b      mu_1  lambda_1      mu_2    loglik df conv
1 0.7547398 0.8414671 0.7793264 0.8414671 -2.393492  3    0
2 0.7547398 0.8414671 0.7793264 0.8414671 -2.393492  3    0
*/
//Creates R compatible input
std::ostream& operator<<(std::ostream& os, const bootstrap& b) noexcept;

} //~namespace sado

#endif // SADO_BOOTSTRAP_H
