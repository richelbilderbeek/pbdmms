#ifndef KEWE_SIMULATION_PARAMETERS_H
#define KEWE_SIMULATION_PARAMETERS_H

namespace kewe {

struct simulation_parameters
{
  int endtime = 10; // End simulation at this generation

  int Nx = 2;         // Number of X alleles
  int Np = 2;         // Number of P alleles
  int Nq = 2;         // Number of Q alleles

  double x0 = 0.5;    // initial x gene
  double p0 = 0.5;    // initial p gene
  double q0 = 0.5;    // initial q gene

  double se = 0.1;    // specificity of mate choice ecological type
  double sm = 0.1;    // specificity of mate choice mating type

  double sc = 0.3;    // width of resource utilization function
  double sk = 1.2;    // width of ecological resource distribution

  double sq = 1.0;    // strength of viability selection on male mating type
  double sv = 0.02;   // width distribution mutation sizes

  double c = 0.0005;  // intensity competition

  double at = 0.05;    // attractivity threshold

  int seed = 123;                                 // Seed for RNG
  int haploid = 1;                                // 1 == run simulation haploid
  int diploid = 0;                                // 1 == run simulation diploid
  int popsize = 10;                // Initial population size

  ///Get the width of ecological resource distribution
  double get_eco_res_distribution_width() const noexcept { return sk; }

  double get_eco_res_util_width() const noexcept { return sc; }

  double get_mate_spec_eco() const noexcept { return se; }

  /// strength of viability selection on male mating type
  /// sigma_s
  double get_viab_sel_male_mate_str() const noexcept { return sq; }


  void set_mate_spec_mate(const double any_sm);

  void set_mate_spec_eco(const double any_se);

  /// strength of viability selection on male mating type
  /// sigma_s
  void set_viab_sel_male_mate_str(const double s) noexcept { sq = s; }

};

///The parameters of figure 3 used in Van Doorn & Weissing 2001
simulation_parameters create_sim_parameters_article_figure_3() noexcept;

///We are really sure branching will occur here
simulation_parameters create_sim_parameters_branching() noexcept;

bool is_valid(const simulation_parameters& p) noexcept;

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

bool will_give_symatric_speciation(const simulation_parameters& p) noexcept;

} //~namespace kewe

#endif // KEWE_SIMULATION_PARAMETERS_H
