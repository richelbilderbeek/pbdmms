#ifndef SADO_BOOTSTRAP_ROW_H
#define SADO_BOOTSTRAP_ROW_H

#include <iosfwd>

namespace sado {

/*

"","ntips","b","mu_1","lambda_1","mu_2","loglik","df","conv","exp_durspec","median_durspec"
"1",2,1.58e-07,0.28,0.46,0.28,-7.01e-07,3,0,1.33,0.92
"2",2,1.58e-07,0.28,0.46,0.28,-7.01e-07,3,0,1.33,0.92
"3",2,1.58e-07,0.28,0.46,0.28,-7.01e-07,3,0,1.33,0.92
*/

class bootstrap_row
{
public:
  bootstrap_row(
    const int ntips,
    const double b,
    const double mu_1,
    const double lambda_1,
    const double mu_2,
    const double loglik,
    const int df,
    const int conv,
    const double exp_durspec,
    const double median_durspec
  );

private:
  const int m_ntips;
  const double m_b;
  const double m_mu_1;
  const double m_lambda_1;
  const double m_mu_2;
  const double m_loglik;
  const int m_df;
  const int m_conv;
  const double m_exp_durspec;
  const double m_median_durspec;

  friend std::ostream& operator<<(std::ostream& os, const bootstrap_row& b) noexcept;
};

std::ostream& operator<<(std::ostream& os, const bootstrap_row& b) noexcept;

} //~namespace sado

#endif // SADO_BOOTSTRAP_ROW_H
