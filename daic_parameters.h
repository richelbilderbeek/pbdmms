#ifndef DAIC_PARAMETERS_H
#define DAIC_PARAMETERS_H

namespace daic {

class parameters
{
public:
  ///Same parameters as DAISEE
  /// @param time duration of simulation in million years, ?crown age
  /// @param mainland_n number of species on mainland, should be 1
  /// @param lac lambda_c, cladogenesis rate
  /// @param mu extinction rate
  /// @param k clade-level carrying capacity. Set k to infinity for non-diversity dependence
  /// @param gam gamma. immigration rate
  /// @param laa lambda_a, anagenesis rate
  parameters(
    const double time,
    const int mainland_n,
    const double lac,
    const double mu,
    const double k,
    const double gam,
    const double laa
  );

  const double m_time;
  const int m_mainland_n;
  const double m_lac;
  const double m_mu;
  const double m_k;
  const double m_gam;
  const double m_laa;
};

parameters create_test_parameters() noexcept;

} //~namespace daic

#endif // DAIC_PARAMETERS_H
