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

  ///Get the phenotypical ecological trait
  double _x() const noexcept { return m_x;}

  ///Get the phenotypical female preference
  double _p() const noexcept { return m_p;}

  ///Get the phenotypical male sexual trait
  double _q() const noexcept { return m_q;}

  ///Get the attractiveness value
  double _a() const noexcept { return m_a;}
  void a_(double A) { m_a = A; }
private:

  ///Phenotypical ecological trait
  double m_x;

  ///Phenotypical female preference
  double m_p;

  ///Phenotypical male sexual trait
  double m_q;

  ///Genotypical ecological trait
  double m_x_gen;
  ///Genotypical female preference
  double m_p_gen;

  ///Genotypical male sexual trait
  double m_q_gen;

  ///Attractiveness value
  double m_a;

  void birth(const indiv& m, const indiv& f);
  friend indiv create_offspring(const indiv& m, const indiv& f);

  friend std::ostream& operator<<(std::ostream& os, const indiv i) noexcept;
};

indiv create_offspring(const indiv& m, const indiv& f);

bool operator==(const indiv& lhs, const indiv& rhs) noexcept;

std::ostream& operator<<(std::ostream& os, const indiv i) noexcept;

} //~namespace sado

#endif // SADO_INDIVIDUAL_H
