#ifndef KEWE_SIMULATION_PARAMETERS_H
#define KEWE_SIMULATION_PARAMETERS_H

#include <iosfwd>
#include "gausser.h"

namespace kewe {

struct simulation_parameters
{
  simulation_parameters(
    const double eco_res_util_width, //= 0.3,
    const double initial_eco_trait, //0.5
    const double initial_fem_pref, //0.5
    const double initial_male_trait, //0.5
    const double mate_spec_eco, //= 0.1,
    const double mate_spec_mate //= 0.1
  );
  //simulation_parameters(const simulation_parameters&) = delete;
  //simulation_parameters& operator=(const simulation_parameters&) = delete;


  double sq = 1.0;    // strength of viability selection on male mating type

  double at = 0.05;    // attractivity threshold

  int seed = 123;                                 // Seed for RNG
  int popsize = 10;                // Initial population size

  ///Get the width of ecological resource distribution
  /// sigma_K
  double get_eco_res_distribution_width() const noexcept { return m_sigma_k; }

  /// width of resource utilization function
  /// sigma_c
  double get_eco_res_util_width() const noexcept;

  const ribi::gausser& get_gauss_eco_res_util_width() const noexcept { return m_gauss_eco_res_util_width; }
  const ribi::gausser& get_gauss_mate_spec_mate() const noexcept { return m_gauss_mate_spec_mate; }
  const ribi::gausser& get_gauss_mate_spec_eco() const noexcept { return m_gauss_mate_spec_eco; }

  /// Number of generations this simulation runs
  int get_end_time() const noexcept { return m_end_time; }

  /// Get the ecological trait all individuals are initialized with at the start of the simulation
  double get_initial_eco_trait() const noexcept { return m_initial_eco_trait; }

  /// Get the female all individuals are initialized with at the start of the simulation
  double get_initial_fem_pref() const noexcept { return m_initial_fem_pref; }

  /// Get the male trait all individuals are initialized with at the start of the simulation
  double get_initial_male_trait() const noexcept { return m_initial_male_trait; }

  /// specificity of mate choice on ecological type
  /// sigma_e
  double get_mate_spec_eco() const noexcept;

  /// specificity of mate choice on mating type
  /// sigma_m
  double get_mate_spec_mate() const noexcept;

  /// strength of viability selection on male mating type
  /// sigma_s
  double get_viab_sel_male_mate_str() const noexcept { return sq; }

  /// width distribution mutation sizes
  /// sigma_v
  double get_mut_distr_width() const noexcept { return sv; }

  void set_eco_res_distribution_width(const double any_sk) { m_sigma_k = any_sk; }

  void set_end_time(const int end_time);

  void set_mut_distr_width(const double any_sigma_v) { sv = any_sigma_v; }

  //void set_eco_res_util_width(const double any_sc) noexcept { m_eco_res_util_width = any_sc; }

  /// strength of viability selection on male mating type
  /// sigma_s
  void set_viab_sel_male_mate_str(const double s) noexcept { sq = s; }

private:

  /// End simulation at this generation
  int m_end_time = 10;



  ///Get the width of ecological resource distribution
  double m_sigma_k = 1.2;

  /// width distribution mutation sizes
  double sv = 0.02;

  int Nx = 1;         // Number of X alleles
  int Np = 1;         // Number of P alleles
  int Nq = 1;         // Number of Q alleles

  /// width of resource utilization function
  const double m_eco_res_util_width;

  const ribi::gausser m_gauss_eco_res_util_width;
  const ribi::gausser m_gauss_mate_spec_eco;
  const ribi::gausser m_gauss_mate_spec_mate;

  /// initial ecological trait
  const double m_initial_eco_trait; //= 0.5;

  /// initial female preference
  const double m_initial_fem_pref; //= 0.5;

  /// initial male trait gene
  const double m_initial_male_trait; // = 0.5;

  /// specificity of mate choice on ecological type
  const double m_mate_spec_eco;

  /// specificity of mate choice on mating type
  const double m_mate_spec_mate;


};

///The parameters of figure 3 used in Van Doorn & Weissing 2001
simulation_parameters create_sim_parameters_article_figure_3() noexcept;

///We are really sure branching will occur here
simulation_parameters create_sim_parameters_branching() noexcept;

///The parameters used in profiling
simulation_parameters create_sim_parameters_profiling() noexcept;

///Random parameters
simulation_parameters create_sim_parameters_random() noexcept;

///Close-to-simplest parameters for testing purposes
simulation_parameters create_test_sim_parameters_haploid_1() noexcept;

inline double get_sigma_c(const simulation_parameters& p) noexcept
{
  return p.get_eco_res_util_width();
}

inline double get_sigma_e(const simulation_parameters& p) noexcept
{
  return p.get_mate_spec_eco();
}

inline double get_sigma_k(const simulation_parameters& p) noexcept
{
  return p.get_eco_res_distribution_width();
}

inline double get_sigma_m(const simulation_parameters& p) noexcept
{
  return p.get_mate_spec_mate();
}

inline double get_sigma_s(const simulation_parameters& p) noexcept
{
  return p.get_viab_sel_male_mate_str();
}

inline double get_sigma_v(const simulation_parameters& p) noexcept
{
  return p.get_mut_distr_width();
}

bool is_valid(const simulation_parameters& p) noexcept;

///Read the eco_res_util_width from the file
double read_eco_res_util_width(const std::string& filename);

///Read the initial_eco_trait from the file
double read_initial_eco_trait(const std::string& filename);

///Read the initial_fem_pref from the file
double read_initial_fem_pref(const std::string& filename);

///Read the initial_male_trait from the file
double read_initial_male_trait(const std::string& filename);

///Read the mate_spec_eco from the file
double read_mate_spec_eco(const std::string& filename);

///Read the mate_spec_mate from the file
double read_mate_spec_mate(const std::string& filename);

///Read the simulation_parameters from a file
simulation_parameters read_simulation_parameters(const std::string& filename);

///Will there be branching on the ecological type?
///This is equation 12 of Van Doorn & Weissing 2001
///Because this simulation has a fixed population size,
///there is no such thing as a birth rate
/// @param birth_rate someting equivalent to a birth rate, we assume it to
///   be 1.0 as on average every individual mates once in this sim
bool will_branch_on_ecotype(
  const simulation_parameters& p,
  const double birth_rate = 1.0
) noexcept;

///Eq 17
bool will_branch_on_male_mating_type(const simulation_parameters& p) noexcept;

bool will_give_sympatric_speciation(const simulation_parameters& p) noexcept;

std::ostream& operator<<(std::ostream& os, const simulation_parameters& p) noexcept;

bool operator==(const simulation_parameters& lhs, const simulation_parameters& rhs) noexcept;
bool operator!=(const simulation_parameters& lhs, const simulation_parameters& rhs) noexcept;

} //~namespace kewe

#endif // KEWE_SIMULATION_PARAMETERS_H
