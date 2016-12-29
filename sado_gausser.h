#ifndef SADO_GAUSSER_H
#define SADO_GAUSSER_H

namespace sado {

/// * raw: just calculating the gaussian
/// * lut: using a look-up table
enum class gausser_implementation { raw, lut };

///Implementation of gausser
class gausser_impl;

class gausser
{
public:
  gausser(
    const double sd,
    const gausser_implementation gi
  );

  double sd() const noexcept;

  ///Get the density at the gaussion at x. Will be 1.0 for x equals 0.0
  double operator()(const double x) const noexcept;

private:
  gausser_impl * const m_impl;
};

} //~namespace sado

#endif // SADO_GAUSSER_H
