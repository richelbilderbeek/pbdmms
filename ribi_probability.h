#ifndef RIBI_PROBABILITY_H
#define RIBI_PROBABILITY_H

#include <iosfwd>

namespace ribi {

///A probability is the chance of something
///happening, which is [0,1]
class probability
{
public:
  probability(const double p);
  double get() const noexcept { return m_p; }

  private:
  double m_p;
};

std::ostream& operator<<(std::ostream& os, const probability p) noexcept;
std::istream& operator>>(std::istream& is, probability& p);
bool operator==(const probability lhs, const probability rhs) noexcept;
bool operator!=(const probability lhs, const probability rhs) noexcept;

} //~namespace ribi

#endif // RIBI_PROBABILITY_H
