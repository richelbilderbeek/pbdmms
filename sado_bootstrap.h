#ifndef SADO_BOOTSTRAP_H
#define SADO_BOOTSTRAP_H

#include <iosfwd>
#include <vector>

#include "sado_bootstrap_row.h"

namespace sado {

struct likelihood;
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
  std::vector<bootstrap_row> m_rows;

  friend std::ostream& operator<<(std::ostream& os, const bootstrap& b) noexcept;
};

std::string create_bootstrap_r_script(
  const std::string& newick,
  const likelihood& max_likelihood,
  const int n,
  const int rng_seed,
  const std::string& csv_filename
);

///Do a bootstrap analysis, by running simulations with the
///maximum likelihood values and obtaining the max likelihoods of those
///runs.
/// @returns n maximum likelihoods
std::vector<bootstrap_row> do_bootstrap(
  const std::string& newick,
  const likelihood& max_likelihood,
  const int n,
  const int rng_seed
);

std::string create_header_bootstrap();

/*

"","ntips","b","mu_1","lambda_1","mu_2","loglik","df","conv","exp_durspec","median_durspec"
"1",2,1.58e-07,0.28,0.46,0.28,-7.01e-07,3,0,1.33,0.92
"2",2,1.58e-07,0.28,0.46,0.28,-7.01e-07,3,0,1.33,0.92
"3",2,1.58e-07,0.28,0.46,0.28,-7.01e-07,3,0,1.33,0.92

*/

std::vector<bootstrap_row> bootstrap_rows_from_file(const std::string& filename);

/*
          b      mu_1  lambda_1      mu_2    loglik df conv
1 0.7547398 0.8414671 0.7793264 0.8414671 -2.393492  3    0
2 0.7547398 0.8414671 0.7793264 0.8414671 -2.393492  3    0
*/
//Creates R compatible input
std::ostream& operator<<(std::ostream& os, const bootstrap& b) noexcept;

} //~namespace sado

#endif // SADO_BOOTSTRAP_H
