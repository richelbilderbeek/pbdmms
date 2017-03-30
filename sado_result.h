#ifndef SADO_RESULT_H
#define SADO_RESULT_H

#include "sado_histogram.h"
#include "sado_std_devs.h"
#include "sado_species.h"

namespace sado
{

class result
{
public:
  /// @param t the time this measurement is taken, in generations from the start
  result(
    const histogram& histp,
    const histogram& histq,
    const histogram& histx,
    const std::vector<species>& any_species,
    const int pop_size,
    const double rhopq,
    const double rhoxp,
    const double rhoxq,
    const std_devs any_std_devs,
    const int t
  );

  /// Individuals grouped in species
  const auto& get_species() const noexcept { return m_species; }

  /// Female preference histogram
  histogram m_histp;

  /// Male sexual trait histogram
  histogram m_histq;

  /// Ecological trait histogram
  histogram m_histx;

  /// Number of individuals
  int m_pop_size;

  double m_rhopq;
  double m_rhoxp;
  double m_rhoxq;

  /// corrected sample standard deviations of p, q and x;
  std_devs m_std_devs;

  ///Time, in generation, first generation has m_t of zero
  int m_t;

private:
  /// Individuals grouped in species
  std::vector<species> m_species;

};

std::ostream& operator<<(std::ostream& os, const result& r) noexcept;

} //~namespace sado

#endif // SADO_RESULT_H
