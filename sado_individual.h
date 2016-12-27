#ifndef SADO_INDIVIDUAL_H
#define SADO_INDIVIDUAL_H

#include <iosfwd>
#include "sado_globals.h"

namespace sado {

class indiv
{
public:
  indiv();
  void init(const double this_x0, const double this_p0, const double this_q0);
  double _x() const noexcept { return x;}
  double _p() const noexcept { return p;}
  double _q() const noexcept { return q;}
  double _a() const noexcept { return a;}
  void a_(double A) { a=A; }
private:
  double x;
  double p;
  double q;
  double m_x_gen;
  double m_p_gen;
  double m_q_gen;
  double a;

  void birth(const indiv& m, const indiv& f);
  friend indiv create_offspring(const indiv& m, const indiv& f);

  friend std::ostream& operator<<(std::ostream& os, const indiv i) noexcept;
};

indiv create_offspring(const indiv& m, const indiv& f);

bool operator==(const indiv& lhs, const indiv& rhs) noexcept;

std::ostream& operator<<(std::ostream& os, const indiv i) noexcept;

} //~namespace sado

#endif // SADO_INDIVIDUAL_H
