#ifndef SADO_INDIVIDUAL_H
#define SADO_INDIVIDUAL_H

#include <list>
#include <vector>
#include "sado_globals.h"
#include "sado_random.h"

namespace sado {

class indiv
{
public:
  indiv();
  void init(const double this_x0, const double this_p0, const double this_q0);
  void birth(const indiv& m, const indiv& f);
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
  std::vector<double> X;
  std::vector<double> P;
  std::vector<double> Q;
  double a;
};

//indiv create_offspring()

bool operator==(const indiv& lhs, const indiv& rhs) noexcept;

} //~namespace sado

#endif // SADO_INDIVIDUAL_H
