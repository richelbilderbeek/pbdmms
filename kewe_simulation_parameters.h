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

  double sc = 0.3;    // unction RJCB: Strength of competition
  double sk = 1.2;    // width of ecological resource distribution

  double sq = 1.0;    // strength of viability selection on male mating type
  double sv = 0.02;   // width distribution mutation sizes

  double c = 0.0005;  // intensity competition

  double at = 0.05;    // attractivity threshold

  int seed = 123;                                 // Seed for RNG
  int haploid = 1;                                // 1 == run simulation haploid
  int diploid = 0;                                // 1 == run simulation diploid
  int popsize = 10;                // Initial population size

  void set_mate_spec_mate(const double any_sm);
  void set_mate_spec_eco(const double any_se);

};

bool is_valid(const simulation_parameters& p) noexcept;

} //~namespace kewe

#endif // KEWE_SIMULATION_PARAMETERS_H
