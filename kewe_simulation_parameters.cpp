#include "kewe_simulation_parameters.h"

#include <cassert>

void kewe::simulation_parameters::set_mate_spec_mate(const double any_sm)
{
  assert(any_sm >= 0.0);
  sm = any_sm;
  assert(is_valid(*this));
}

void kewe::simulation_parameters::set_mate_spec_eco(const double any_se)
{
  assert(any_se >= 0.0);
  se = any_se;
  assert(is_valid(*this));
}

bool kewe::is_valid(const simulation_parameters& p) noexcept //!OCLINT
{
  return p.endtime > 0
    && p.Nx > 0
    && p.Np > 0
    && p.Nq > 0
    && p.se >= 0.0
    && p.sm >= 0.0
    && p.sc >= 0.0
    && p.sk >= 0.0
    && p.sq >= 0.0
    && p.sv >= 0.0
    && p.c >= 0.0
    && p.at >= 0.0
    && (p.haploid ^ p.diploid)
    && p.popsize > 0
  ;
}
