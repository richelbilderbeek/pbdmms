#ifndef KEWE_GAUSSER_H
#define KEWE_GAUSSER_H

#include <vector>

namespace kewe {

///Quick calculation of Guass
class gausser
{
public:
  ///@param sd standard deviation of the gaussian
  gausser(const double sd);

  ///Fast function
  double operator()(const double x) const noexcept;

  ///Calls gauss function
  double get_precise(const double x) const noexcept;

  ///Uses gausser
  double get_fast(const double x) const noexcept;

private:
  const double m_sd;

  ///Lookup table
  const std::vector<double> m_lut;

  ///Create the LUT
  std::vector<double> create_lut(const double sd, const int sz);
};


} //~namespace kewe


#endif // KEWE_GAUSSER_H
