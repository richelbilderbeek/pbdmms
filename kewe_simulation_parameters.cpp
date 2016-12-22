#include "kewe_simulation_parameters.h"

#include <cassert>

kewe::simulation_parameters kewe::create_sim_parameters_article_figure_3() noexcept
{
  simulation_parameters p;
  p.set_end_time(4000);
  p.Nx = 2;
  p.Np = 2;
  p.Nq = 2;
  p.x0 = 0.5;
  p.p0 = 0.5;
  p.q0 = 0.5;
  p.se = 0.6;
  p.sm = 0.2;
  p.sc = 0.4;
  p.set_eco_res_distribution_width(1.2);
  p.sq = 1.0;
  p.sv = 0.02;
  p.c = 0.0005;
  p.at = 0.05;
  p.seed = 123;
  p.haploid = 1;
  p.diploid = 0;
  p.popsize = 1000;
  //eta, cost of mate choice: 1.0
  //epsilon,  cost of mate choice / number of males: 1.0 / number of males
  return p;
}

kewe::simulation_parameters kewe::create_sim_parameters_branching() noexcept
{
  simulation_parameters p;
  p.set_end_time(4000);
  p.Nx = 1;
  p.Np = 1;
  p.Nq = 1;
  p.x0 = 0.5;
  p.p0 = 0.5;
  p.q0 = 0.5;
  p.se = 0.4; 
  p.sm = 0.05;
  p.sc = 0.5; 
  p.set_eco_res_distribution_width(1.2);
  p.sq = 0.25;
  p.sv = 0.02;
  p.c = 0.0005;
  p.at = 0.05;
  p.seed = 123;
  p.haploid = 1;
  p.diploid = 0;
  p.popsize = 1000;
  
  assert(will_branch_on_ecotype(p));
  assert(will_branch_on_male_mating_type(p));
  assert(will_give_sympatric_speciation(p));

  //eta, cost of mate choice: 1.0
  //epsilon,  cost of mate choice / number of males: 1.0 / number of males
  return p;
}

void kewe::simulation_parameters::set_end_time(const int any_end_time)
{
  assert(any_end_time > 0);
  m_end_time = any_end_time;
  assert(is_valid(*this));
}

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

void kewe::simulation_parameters::set_ploidy(const ploidy p) noexcept
{
  if (p == ploidy::diploid)
  {
    this->diploid = 1;
    this->haploid = 0;
  }
  else
  {
    assert(p == ploidy::haploid);
    this->diploid = 0;
    this->haploid = 1;
  }
}


bool kewe::is_valid(const simulation_parameters& p) noexcept //!OCLINT
{
  return p.get_end_time() > 0
    && p.Nx > 0
    && p.Np > 0
    && p.Nq > 0
    && p.se >= 0.0
    && p.sm >= 0.0
    && p.sc >= 0.0
    && p.get_eco_res_distribution_width() >= 0.0
    && p.sq >= 0.0
    && p.sv >= 0.0
    && p.c >= 0.0
    && p.at >= 0.0
    && (p.haploid ^ p.diploid)
    && p.popsize > 0
  ;
}

bool kewe::will_branch_on_ecotype(
  const simulation_parameters& p,
  const double birth_rate
) noexcept
{
  const double sk{p.get_eco_res_distribution_width()};
  const double sc{p.get_eco_res_util_width()};
  const double se{p.get_mate_spec_eco()};
  const double b{birth_rate};
  const double lhs{(sk * sk) / (sc * sc)};
  const double rhs_term_2{(1.0 / (b - 2.0))};
  const double rhs_term_3{(sk * sk) / (se * se)};
  const double rhs{1.0 +  (rhs_term_2 * rhs_term_3)};
  return lhs > rhs;
}

bool kewe::will_branch_on_male_mating_type(const simulation_parameters& p) noexcept
{
  //STUB
  return p.c > -1.0;
}

bool kewe::will_give_sympatric_speciation(const simulation_parameters& p) noexcept
{
  //STUB
  return p.c > -1.0;
}

