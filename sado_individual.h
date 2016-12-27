#ifndef SADO_INDIVIDUAL_H
#define SADO_INDIVIDUAL_H

#include "sado_globals.h"
#include "sado_random.h"

namespace sado {

class indiv
{
public:
  indiv();
  void init(const double this_x0, const double this_p0, const double this_q0);
  void print() const noexcept;
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
};

indiv create_offspring(const indiv& m, const indiv& f);

bool operator==(const indiv& lhs, const indiv& rhs) noexcept;

} //~namespace sado

#endif // SADO_INDIVIDUAL_H
