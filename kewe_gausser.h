#ifndef KEWE_GAUSSER_H
#define KEWE_GAUSSER_H

namespace kewe {

///Quick calculation of Guass
class gausser
{
public:
  ///@param sd standard deviation of the gaussian
  ///@tolerance percentage the values may be off, set to 0.1% by default
  gausser(const double sd, const double tolerance = 0.001);

  double operator()(const double x) const noexcept;

private:
  const double m_sd;
  const double m_tolerance;
};

} //~namespace kewe


#endif // KEWE_GAUSSER_H
