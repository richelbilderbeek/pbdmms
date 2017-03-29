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
  bootstrap(const likelihood& max_likelihood, const int n);
private:
  std::vector<likelihood> m_likelihoods;

  friend std::ostream& operator<<(std::ostream& os, const bootstrap& b) noexcept;
};

///Do a bootstrap analysis, by running simulations with the
///maximum likelihood values and obtaining the max likelihoods of those
///runs.
/// @returns n maximum likelihoods
std::vector<likelihood> do_bootstrap(const likelihood& max_likelihood, const int n);

std::ostream& operator<<(std::ostream& os, const bootstrap& b) noexcept;

} //~namespace sado

#endif // SADO_BOOTSTRAP_H
