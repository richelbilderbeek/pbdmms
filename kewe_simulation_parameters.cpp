#include "kewe_simulation_parameters.h"

#include <cassert>
#include <iostream>
#include <random>
#include <stdexcept>

#include "file_to_vector.h"
#include "seperate_string.h"

kewe::simulation_parameters::simulation_parameters(
  const double eco_res_distr_sd,
  const double eco_res_util_sd,
  const double initial_eco_trait,
  const double initial_fem_pref,
  const double initial_male_trait,
  const double mate_spec_eco,
  const double mate_spec_mate
)
  : m_eco_res_distr_sd{eco_res_distr_sd},
    m_eco_res_util_width{eco_res_util_sd},
    m_gauss_eco_res_distr_sd(eco_res_distr_sd),
    m_gauss_eco_res_util_sd(eco_res_util_sd),
    m_gauss_mate_spec_eco(mate_spec_eco),
    m_gauss_mate_spec_mate(mate_spec_mate),
    m_initial_eco_trait{initial_eco_trait},
    m_initial_fem_pref{initial_fem_pref},
    m_initial_male_trait{initial_male_trait},
    m_mate_spec_eco{mate_spec_eco},
    m_mate_spec_mate{mate_spec_mate}
{
  assert(eco_res_distr_sd > 0.0); //gausser will already throw
  assert(m_eco_res_util_width > 0.0); //gausser will already throw
  assert(m_mate_spec_eco > 0.0); //gausser will already throw
  assert(m_mate_spec_mate > 0.0); //gausser will already throw

  assert(m_gauss_eco_res_util_sd.sd() == eco_res_util_sd);
  assert(m_gauss_mate_spec_eco.sd() == mate_spec_eco);
  assert(m_gauss_mate_spec_mate.sd() == mate_spec_mate);
  assert(m_gauss_eco_res_distr_sd.sd() == eco_res_distr_sd);
}


kewe::simulation_parameters kewe::create_sim_parameters_article_figure_3() noexcept
{
  const double eco_res_distr_sd{1.2};
  const double eco_res_util_width{0.4};
  const double initial_eco_trait{0.5};
  const double initial_fem_pref{0.5};
  const double initial_male_trait{0.5};
  const double mate_spec_eco{0.6};
  const double mate_spec_mate{0.2};
  simulation_parameters p(
    eco_res_distr_sd,
    eco_res_util_width,
    initial_eco_trait,
    initial_fem_pref,
    initial_male_trait,
    mate_spec_eco,
    mate_spec_mate
  );
  p.set_end_time(4000);
  p.sq = 1.0;
  p.set_mut_distr_width(0.02);
  p.at = 0.05;
  p.seed = 123;
  p.popsize = 1000;
  //eta, cost of mate choice: 1.0
  //epsilon,  cost of mate choice / number of males: 1.0 / number of males
  return p;
}

kewe::simulation_parameters kewe::create_sim_parameters_branching() noexcept
{
  const double eco_res_distr_sd{1.2};
  const double eco_res_util_width{0.5};
  const double initial_eco_trait{0.5};
  const double initial_fem_pref{0.5};
  const double initial_male_trait{0.5};
  const double mate_spec_eco{0.4};
  const double mate_spec_mate{0.05};
  simulation_parameters p(
    eco_res_distr_sd,
    eco_res_util_width,
    initial_eco_trait,
    initial_fem_pref,
    initial_male_trait,
    mate_spec_eco,
    mate_spec_mate
  );
  p.set_end_time(10000);
  p.sq = 0.25;
  p.set_mut_distr_width(0.02);
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
  const double eco_res_distr_sd{1.2};
  const double eco_res_util_width{0.3};
  const double initial_eco_trait{0.5};
  const double initial_fem_pref{0.5};
  const double initial_male_trait{0.5};
  const double mate_spec_eco{0.1};
  const double mate_spec_mate{0.01};
  simulation_parameters p(
    eco_res_distr_sd,
    eco_res_util_width,
    initial_eco_trait,
    initial_fem_pref,
    initial_male_trait,
    mate_spec_eco,
    mate_spec_mate
  );
  p.set_end_time(100); // End simulation at this generation
  p.sq = 1.0;    // strength of viability selection on male mating type
  p.set_mut_distr_width(0.02);   // width distribution mutation sizes
  p.at = 0.05;    // attractivity threshold
  p.seed = 123;
  p.popsize = 3000;
  return p;
}

kewe::simulation_parameters kewe::create_sim_parameters_random() noexcept
{
  std::random_device rd;   // non-deterministic generator
  std::mt19937 gen(rd());
  std::uniform_real_distribution<double> dist(0.0, 1.0);

  const double eco_res_distr_sd{1.2};
  const double eco_res_util_width{dist(gen)};
  const double initial_eco_trait{0.5};
  const double initial_fem_pref{0.5};
  const double initial_male_trait{0.5};
  const double mate_spec_eco{dist(gen)};
  const double mate_spec_mate{dist(gen)};
  simulation_parameters p(
    eco_res_distr_sd,
    eco_res_util_width,
    initial_eco_trait,
    initial_fem_pref,
    initial_male_trait,
    mate_spec_eco,
    mate_spec_mate
  );
  p.set_end_time(5000); // End simulation at this generation
  p.sq = dist(gen);    // strength of viability selection on male mating type
  p.set_mut_distr_width(dist(gen));   // width distribution mutation sizes
  p.at = dist(gen) * 0.1;
  p.seed = 123;                                 // Seed for RNG
  p.popsize = 1000;                // Initial population size
  return p;

}


kewe::simulation_parameters kewe::create_test_sim_parameters_haploid_1() noexcept
{
  const double eco_res_distr_sd{1.2};
  const double eco_res_util_sd{0.3};
  const double initial_eco_trait{0.5};
  const double initial_fem_pref{0.5};
  const double initial_male_trait{0.5};
  const double mate_spec_eco{0.1};
  const double mate_spec_mate{0.1};
  simulation_parameters p(
    eco_res_distr_sd,
    eco_res_util_sd,
    initial_eco_trait,
    initial_fem_pref,
    initial_male_trait,
    mate_spec_eco,
    mate_spec_mate
  );
  p.set_end_time(10); // End simulation at this generation
  p.sq = 1.0;    // strength of viability selection on male mating type
  p.set_mut_distr_width(0.02);   // width distribution mutation sizes
  p.at = 0.05;    // attractivity threshold
  p.seed = 123;                                 // Seed for RNG
  p.popsize = 10;                // Initial population size
  return p;
}

double kewe::simulation_parameters::get_eco_res_util_width() const noexcept
{
  assert(m_gauss_eco_res_util_sd.sd() == m_eco_res_util_width);
  return m_eco_res_util_width;
}

double kewe::simulation_parameters::get_mate_spec_eco() const noexcept
{
  assert(m_gauss_mate_spec_eco.sd() == m_mate_spec_eco);
  return m_mate_spec_eco;
}

double kewe::simulation_parameters::get_mate_spec_mate() const noexcept
{
  assert(m_gauss_mate_spec_mate.sd() == m_mate_spec_mate);
  return m_mate_spec_mate;
}

double kewe::read_eco_res_distribution_width(const std::string& filename)
{
  const auto lines = file_to_vector(filename);
  for (const std::string& line: lines)
  {
    const std::vector<std::string> v{seperate_string(line, ' ')};
    if(v[0] == "sk") { return std::stod(v[1]); }
  }
  throw std::runtime_error("parameter sk not found");
}

double kewe::read_eco_res_util_width(const std::string& filename)
{
  const auto lines = file_to_vector(filename);
  for (const std::string& line: lines)
  {
    const std::vector<std::string> v{seperate_string(line, ' ')};
    if(v[0] == "sc") { return std::stod(v[1]); }
  }
  throw std::runtime_error("parameter sc not found");
}

double kewe::read_initial_eco_trait(const std::string& filename)
{
  const auto lines = file_to_vector(filename);
  for (const std::string& line: lines)
  {
    const std::vector<std::string> v{seperate_string(line, ' ')};
    if(v[0] == "type0")
    {
      if (v.size() != 4)
      {
        throw std::invalid_argument("must supply 'type0' and three values");
      }
      return std::stod(v.at(1));
    }
  }
  throw std::runtime_error("parameter initial_eco_trait not found");
}

double kewe::read_initial_fem_pref(const std::string& filename)
{
  const auto lines = file_to_vector(filename);
  for (const std::string& line: lines)
  {
    const std::vector<std::string> v{seperate_string(line, ' ')};
    if(v[0] == "type0")
    {
      if (v.size() != 4)
      {
        throw std::invalid_argument("must supply 'type0' and three values");
      }
      return std::stod(v.at(2));
    }
  }
  throw std::runtime_error("parameter initial_fem_pref not found");
}

double kewe::read_initial_male_trait(const std::string& filename)
{
  const auto lines = file_to_vector(filename);
  for (const std::string& line: lines)
  {
    const std::vector<std::string> v{seperate_string(line, ' ')};
    if(v[0] == "type0")
    {
      if (v.size() != 4)
      {
        throw std::invalid_argument("must supply 'type0' and three values");
      }
      return std::stod(v.at(3));
    }
  }
  throw std::runtime_error("parameter initial_male_trait not found");
}

double kewe::read_mate_spec_eco(const std::string& filename)
{
  const auto lines = file_to_vector(filename);
  for (const std::string& line: lines)
  {
    const std::vector<std::string> v{seperate_string(line, ' ')};
    if(v[0] == "se") { return std::stod(v[1]); }
  }
  throw std::runtime_error("parameter se not found");
}

double kewe::read_mate_spec_mate(const std::string& filename)
{
  const auto lines = file_to_vector(filename);
  for (const std::string& line: lines)
  {
    const std::vector<std::string> v{seperate_string(line, ' ')};
    if(v[0] == "sm") { return std::stod(v[1]); }
  }
  throw std::runtime_error("parameter sm not found");
}


kewe::simulation_parameters kewe::read_simulation_parameters(const std::string& filename)
{
  return simulation_parameters(
    read_eco_res_distribution_width(filename),
    read_eco_res_util_width(filename),
    read_initial_eco_trait(filename),
    read_initial_fem_pref(filename),
    read_initial_male_trait(filename),
    read_mate_spec_eco(filename),
    read_mate_spec_mate(filename)
  );
}

void kewe::simulation_parameters::set_end_time(const int any_end_time)
{
  assert(any_end_time > 0);
  m_end_time = any_end_time;
  assert(is_valid(*this));
}

bool kewe::is_valid(const simulation_parameters& p) noexcept //!OCLINT
{
  return p.get_end_time() > 0
    && p.get_mate_spec_eco() > 0.0
    && p.get_mate_spec_mate() > 0.0
    && p.get_eco_res_util_width() >= 0.0
    && p.get_eco_res_distribution_width() >= 0.0
    && p.sq >= 0.0
    && p.get_mut_distr_width() >= 0.0
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
  return p.get_eco_res_distribution_width() > -1.0;
}

bool kewe::will_give_sympatric_speciation(const simulation_parameters& p) noexcept
{
  //STUB
  return p.get_eco_res_distribution_width() > -1.0;
}

std::ostream& kewe::operator<<(std::ostream& os, const simulation_parameters& p) noexcept
{
  os
      << "x0: " << p.get_initial_eco_trait() << '\n'
      << "p0: " << p.get_initial_fem_pref() << '\n'
      << "q0: " << p.get_initial_male_trait() << '\n'
      << "se: " << p.get_mate_spec_eco() << '\n'
      << "sm: " << p.get_mate_spec_mate() << '\n'
      << "sc: " << p.get_eco_res_util_width() << '\n'
      << "sq: " << p.sq << '\n'
      << "at: " << p.at << '\n'
      << "seed: " << p.seed << '\n'
      << "popsize: " << p.popsize << '\n'
      << "end_time: " << p.get_end_time() << '\n'
      << "sk: " << p.get_eco_res_distribution_width() << '\n'
      << "sv: " << p.get_mut_distr_width();

  return os;
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

