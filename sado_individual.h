#ifndef SADO_INDIVIDUAL_H
#define SADO_INDIVIDUAL_H

#include <iosfwd>
#include <vector>
#include "sado_fwd.h"
#include "sado_id.h"

namespace sado
{

class individual
{
public:
  /// Note: p_gen, q_gen and x_gen exist solely to allow for the initialization bug
  ///Ensures all individuals have a unique ID
  explicit individual(
    const id mother_id = phyg::create_null_id(),
    const id father_id = phyg::create_null_id(),
    const double p = 0.0,
    const double q = 0.0,
    const double x = 0.0,
    const double p_gen = 0.0,
    const double q_gen = 0.0,
    const double x_gen = 0.0
  );
  /// Get the phenotypical ecological trait
  double get_x() const noexcept { return m_x; }

  /// Get the phenotypical female preference
  double get_p() const noexcept { return m_p; }

  /// Get the phenotypical male sexual trait
  double get_q() const noexcept { return m_q; }

  /// Get the individual its ID
  id get_id() const noexcept { return m_id; }

  /// Get the individual's mother's ID
  id get_mother_id() const noexcept { return m_id_mother; }

  /// Get the individual's father's ID
  id get_father_id() const noexcept { return m_id_father; }

private:

  id m_id;
  id m_id_mother;
  id m_id_father;

  /// Phenotypical female preference
  double m_p;

  /// Phenotypical male sexual trait
  double m_q;

  /// Phenotypical ecological trait
  double m_x;

  /// Genotypical female preference. Equal to phenotype, except in the case of
  /// the initialization bug
  double m_p_gen;

  /// Genotypical male sexual trait. Equal to phenotype, except in the case of
  /// the initialization bug
  double m_q_gen;

  /// Genotypical ecological trait. Equal to phenotype, except in the case of
  /// the initialization bug
  double m_x_gen;

  friend individual
  create_offspring(const individual& m, const individual& f, const parameters& p);

  //Made this a friend to not expose m_{p,q,x}_gen
  friend population create_initial_population(const parameters& p);

  friend std::ostream &operator<<(std::ostream &os, const individual i) noexcept;

};

///Checks if all individuals have a unique ID
bool all_have_unique_ids(const std::vector<individual>& v);

/// Initialize individual with original bug
individual create_init_with_bug(
    const double this_x0,
    const double this_p0,
    const double this_q0,
    const parameters& p);

individual create_offspring(const individual& m, const individual& f, const parameters& p);

bool operator==(const individual& lhs, const individual& rhs) noexcept;
bool operator!=(const individual& lhs, const individual& rhs) noexcept;

///Sort individuals by their ID
bool operator<(const individual& lhs, const individual& rhs) noexcept;

std::ostream &operator<<(std::ostream &os, const individual i) noexcept;

} //~namespace sado

#endif // SADO_INDIVIDUAL_H
