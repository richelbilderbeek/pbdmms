#ifndef SADO_LIKELIHOOD_H
#define SADO_LIKELIHOOD_H

#include <iosfwd>
#include <string>

namespace sado {

///The likelihood that is returned from
///the R function PBD::pbd_ML
class likelihood
{
public:
  likelihood(
    const double b,
    const int conv,
    const int df,
    const double lambda_1,
    const double loglik,
    const double mu_1,
    const double mu_2
  );
  auto get_b() const noexcept { return m_b; }
  auto get_conv() const noexcept { return m_conv; }
  auto get_df() const noexcept { return m_df; }
  auto get_lambda_1() const noexcept { return m_lambda_1; }
  auto get_loglik() const noexcept { return m_loglik; }
  auto get_mu_1() const noexcept { return m_mu_1; }
  auto get_mu_2() const noexcept { return m_mu_2; }

private:
  double m_b;
  int m_conv;
  int m_df;
  double m_lambda_1;
  double m_loglik;
  double m_mu_1;
  double m_mu_2;

};

///Read the likelihood from a file
likelihood likelihood_from_file(const std::string& filename);

///Calculate the maximum likelihood from a Newick
likelihood calc_max_likelihood(const std::string& newick);

std::ostream& operator<<(std::ostream& os, const likelihood& l) noexcept;

} //~namespace sado

#endif // SADO_LIKELIHOOD_H
