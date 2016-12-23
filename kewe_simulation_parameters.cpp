#include "kewe_simulation_parameters.h"

#include <cassert>
#include <random>

kewe::simulation_parameters kewe::create_sim_parameters_article_figure_3() noexcept
{
  simulation_parameters p;
  p.set_end_time(4000);
  p.x0 = 0.5;
  p.p0 = 0.5;
  p.q0 = 0.5;
  p.se = 0.6;
  p.sm = 0.2;
  p.sc = 0.4;
  p.set_eco_res_distribution_width(1.2);
  p.sq = 1.0;
  p.set_mut_distr_width(0.02);
  p.c = 0.0005;
  p.at = 0.05;
  p.seed = 123;
  p.popsize = 1000;
  //eta, cost of mate choice: 1.0
  //epsilon,  cost of mate choice / number of males: 1.0 / number of males
  return p;
}

kewe::simulation_parameters kewe::create_sim_parameters_branching() noexcept
{
  simulation_parameters p;
  p.set_end_time(10000);
  p.x0 = 0.5;
  p.p0 = 0.5;
  p.q0 = 0.5;
  p.se = 0.4; 
  p.sm = 0.05;
  p.sc = 0.5; 
  p.set_eco_res_distribution_width(1.2);
  p.sq = 0.25;
  p.set_mut_distr_width(0.02);
  p.c = 0.0005;
  p.at = 0.05;
  p.seed = 123;
  p.popsize = 100;
  
  assert(will_branch_on_ecotype(p));
  assert(will_branch_on_male_mating_type(p));
  assert(will_give_sympatric_speciation(p));

  //eta, cost of mate choice: 1.0
  //epsilon,  cost of mate choice / number of males: 1.0 / number of males
  return p;
}

kewe::simulation_parameters kewe::create_sim_parameters_profiling() noexcept
{
  simulation_parameters p;
  p.set_end_time(1000); // End simulation at this generation
  p.x0 = 0.5;    // initial x gene
  p.p0 = 0.5;    // initial p gene
  p.q0 = 0.5;    // initial q gene
  p.se = 0.1;    // specificity of mate choice ecological type
  p.sm = 0.1;    // specificity of mate choice mating type
  p.sc = 0.3;    // unction RJCB: Strength of competition
  p.set_eco_res_distribution_width(1.2);
  p.sq = 1.0;    // strength of viability selection on male mating type
  p.set_mut_distr_width(0.02);   // width distribution mutation sizes
  p.c = 0.0005;  // intensity competition
  p.at = 0.05;    // attractivity threshold
  p.seed = 123;
  p.popsize = 1000;                // Initial population size
  return p;
}

kewe::simulation_parameters kewe::create_sim_parameters_random() noexcept
{
  std::random_device rd;   // non-deterministic generator
  std::mt19937 gen(rd());
  simulation_parameters p;
  std::uniform_real_distribution<double> dist(0.0, 1.0);
  p.set_end_time(10000); // End simulation at this generation
  p.x0 = 0.5;    // initial x gene
  p.p0 = 0.5;    // initial p gene
  p.q0 = 0.5;    // initial q gene
  p.se = dist(gen);    // specificity of mate choice ecological type
  p.sm = dist(gen);    // specificity of mate choice mating type
  p.sc = dist(gen);    // unction RJCB: Strength of competition
  p.set_eco_res_distribution_width(dist(gen));
  p.sq = dist(gen);    // strength of viability selection on male mating type
  p.set_mut_distr_width(dist(gen));   // width distribution mutation sizes
  p.c = dist(gen);  // intensity competition
  p.at = dist(gen);    // attractivity threshold
  p.seed = 123;                                 // Seed for RNG
  p.popsize = 10000;                // Initial population size
  return p;

}


kewe::simulation_parameters kewe::create_test_sim_parameters_haploid_1() noexcept
{
  simulation_parameters p;
  p.set_end_time(10); // End simulation at this generation
  p.x0 = 0.5;    // initial x gene
  p.p0 = 0.5;    // initial p gene
  p.q0 = 0.5;    // initial q gene
  p.se = 0.1;    // specificity of mate choice ecological type
  p.sm = 0.1;    // specificity of mate choice mating type
  p.sc = 0.3;    // unction RJCB: Strength of competition
  p.set_eco_res_distribution_width(1.2);
  p.sq = 1.0;    // strength of viability selection on male mating type
  p.set_mut_distr_width(0.02);   // width distribution mutation sizes
  p.c = 0.0005;  // intensity competition
  p.at = 0.05;    // attractivity threshold
  p.seed = 123;                                 // Seed for RNG
  p.popsize = 10;                // Initial population size
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

bool kewe::is_valid(const simulation_parameters& p) noexcept //!OCLINT
{
  return p.get_end_time() > 0
    && p.se >= 0.0
    && p.sm >= 0.0
    && p.sc >= 0.0
    && p.get_eco_res_distribution_width() >= 0.0
    && p.sq >= 0.0
    && p.get_mut_distr_width() >= 0.0
    && p.c >= 0.0
    && p.at >= 0.0
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

bool kewe::operator==(
  const simulation_parameters& /* lhs */,
  const simulation_parameters& /* rhs */
) noexcept
{
  //STUB
  return true;
}

bool kewe::operator!=(const simulation_parameters& lhs, const simulation_parameters& rhs) noexcept
{
  return !(lhs == rhs);
}

